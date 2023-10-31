package main

import (
	"context"
	"fmt"
	"log"
	"os"
	"os/exec"
	"regexp"
	"strconv"
	"sync"
	"time"

	"github.com/schollz/progressbar/v3"
)

// CacheResults 定义了我们要提取的性能指标
type CacheResults struct {
	L1Reads               int
	L1ReadMisses          int
	L1Writes              int
	L1WriteMisses         int
	L1MissRate            float64
	Swaps                 int
	VictimCacheWritebacks int
	L2Reads               int
	L2ReadMisses          int
	L2Writes              int
	L2WriteMisses         int
	L2MissRate            float64
	L2Writebacks          int
	TotalMemoryTraffic    int
	AverageAccessTime     float64
}

func extractInt(output string, regexPattern string) int {
	r := regexp.MustCompile(regexPattern)
	match := r.FindStringSubmatch(output)
	if len(match) > 1 {
		val, _ := strconv.Atoi(match[1])
		return val
	}
	return 0
}

func extractFloat(output string, regexPattern string) float64 {
	r := regexp.MustCompile(regexPattern)
	match := r.FindStringSubmatch(output)
	if len(match) > 1 {
		val, _ := strconv.ParseFloat(match[1], 64)
		return val
	}
	return 0
}

func ExtractMetrics(output string) CacheResults {
	return CacheResults{
		L1Reads:               extractInt(output, `number of L1 reads:\s+(\d+)`),
		L1ReadMisses:          extractInt(output, `number of L1 read misses:\s+(\d+)`),
		L1Writes:              extractInt(output, `number of L1 writes:\s+(\d+)`),
		L1WriteMisses:         extractInt(output, `number of L1 write misses:\s+(\d+)`),
		L1MissRate:            extractFloat(output, `L1 miss rate:\s+([\d.]+)`),
		Swaps:                 extractInt(output, `number of swaps:\s+(\d+)`),
		VictimCacheWritebacks: extractInt(output, `number of victim cache writeback:\s+(\d+)`),
		L2Reads:               extractInt(output, `number of L2 reads:\s+(\d+)`),
		L2ReadMisses:          extractInt(output, `number of L2 read misses:\s+(\d+)`),
		L2Writes:              extractInt(output, `number of L2 writes:\s+(\d+)`),
		L2WriteMisses:         extractInt(output, `number of L2 write misses:\s+(\d+)`),
		L2MissRate:            extractFloat(output, `L2 miss rate:\s+([\d.]+)`),
		L2Writebacks:          extractInt(output, `number of L2 writebacks:\s+(\d+)`),
		TotalMemoryTraffic:    extractInt(output, `total memory traffic:\s+(\d+)`),
		AverageAccessTime:     extractFloat(output, `average access time:\s+([\d.]+)`),
	}
}

type Parameters struct {
	BLOCKSIZE,
	L1_SIZE,
	L2_SIZE,
	L1_ASSOC,
	L2_ASSOC,
	Victim_Cache_SIZE int

	trace_file string
}

func runWrapper(ctx context.Context, pa chan Parameters, r chan<- string, m chan<- struct{}) {
	for {
		select {
		case <-ctx.Done():
			time.Sleep(4 * time.Second)
			return
		case p := <-pa:
			m <- struct{}{}
			// Check whether Has
			str := fmt.Sprintf("%v %v %v %v %v %v %v", p.BLOCKSIZE, p.L1_SIZE, p.L2_SIZE, p.L1_ASSOC, p.L2_ASSOC, p.Victim_Cache_SIZE, p.trace_file)
			cmd := exec.Command("grep", str, "result.txt")
			if _, err := cmd.CombinedOutput(); err != nil {
				// Done
				run(p, r, m)
			} else {
				// log.Printf("Done with %v", str)
			}
		}
	}
}

func run(p Parameters, results chan<- string, msg chan<- struct{}) {
	cmd := exec.Command("./sim_cache",
		strconv.Itoa(p.BLOCKSIZE),
		strconv.Itoa(p.L1_SIZE),
		strconv.Itoa(p.L2_SIZE),
		strconv.Itoa(p.L1_ASSOC),
		strconv.Itoa(p.L2_ASSOC),
		strconv.Itoa(p.Victim_Cache_SIZE),
		p.trace_file,
	)
	output, err := cmd.CombinedOutput()
	cr := ExtractMetrics(string(output))
	if err != nil {
		results <- fmt.Sprintf("Error running with parameters %+v: %v : %+v", p, err, string(output))
		msg <- struct{}{}
		return
	}
	msg <- struct{}{}
	results <- fmt.Sprintf(
		"%v %v %v %v %v %v %v %v %v %v\n", p.BLOCKSIZE, p.L1_SIZE, p.L2_SIZE, p.L1_ASSOC, p.L2_ASSOC, p.Victim_Cache_SIZE, p.trace_file, cr.L1MissRate, cr.L2MissRate, cr.AverageAccessTime)
}

func main() {

	MaxProc := 6

	file, err := os.OpenFile("result.txt", os.O_APPEND|os.O_RDWR, 0666)
	if err != nil {
		panic(err)
	}
	defer file.Close()

	ps := []Parameters{
		// {BLOCKSIZE: 64, L1_SIZE: 8192, L2_SIZE: 16384, L1_ASSOC: 2, L2_ASSOC: 4, Victim_Cache_SIZE: 1024, trace_file: "perl_trace.txt"},
		// {BLOCKSIZE: 64, L1_SIZE: 8192, L2_SIZE: 16384, L1_ASSOC: 2, L2_ASSOC: 4, Victim_Cache_SIZE: 1024, trace_file: "perl_trace.txt"},
		// {BLOCKSIZE: 64, L1_SIZE: 8192, L2_SIZE: 16384, L1_ASSOC: 2, L2_ASSOC: 4, Victim_Cache_SIZE: 1024, trace_file: "perl_trace.txt"},
		// {BLOCKSIZE: 64, L1_SIZE: 8192, L2_SIZE: 16384, L1_ASSOC: 2, L2_ASSOC: 4, Victim_Cache_SIZE: 1024, trace_file: "perl_trace.txt"},
		// {BLOCKSIZE: 64, L1_SIZE: 8192, L2_SIZE: 16384, L1_ASSOC: 2, L2_ASSOC: 4, Victim_Cache_SIZE: 1024, trace_file: "perl_trace.txt"},
		// {BLOCKSIZE: 64, L1_SIZE: 8192, L2_SIZE: 16384, L1_ASSOC: 2, L2_ASSOC: 4, Victim_Cache_SIZE: 1024, trace_file: "perl_trace.txt"},
		// {BLOCKSIZE: 64, L1_SIZE: 8192, L2_SIZE: 16384, L1_ASSOC: 2, L2_ASSOC: 4, Victim_Cache_SIZE: 1024, trace_file: "perl_trace.txt"},
		// {BLOCKSIZE: 64, L1_SIZE: 8192, L2_SIZE: 16384, L1_ASSOC: 2, L2_ASSOC: 4, Victim_Cache_SIZE: 1024, trace_file: "perl_trace.txt"},
		// {BLOCKSIZE: 64, L1_SIZE: 8192, L2_SIZE: 16384, L1_ASSOC: 2, L2_ASSOC: 4, Victim_Cache_SIZE: 1024, trace_file: "perl_trace.txt"},
	} // List of parameter combinations

	// 1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288

	bs := []int{16, 32, 64, 128, 256}

	l1s := []int{1024, 2048, 4096, 8192, 16384, 32768, 65536}
	l1a := []int{2, 4, 8, 16, 32}

	l2s := []int{512, 1024, 2048, 4096, 8192, 16384, 32768, 65536}
	l2a := []int{2, 4, 8, 16, 32}

	vics := []int{512, 1024, 2048, 4096}
	tf := []string{"gcc_trace.txt", "perl_trace.txt", "go_trace.txt"}

	for _, b := range bs {
		for _, a1 := range l1a {
			for _, l1 := range l1s {
				if l1 <= a1*b {
					continue
				}
				for _, a2 := range l2a {
					for _, l2 := range l2s {
						if l2 <= a2*b {
							continue
						}
						for _, v := range vics {
							if v > l1 {
								break
							}
							for _, t := range tf {
								ps = append(ps,
									Parameters{BLOCKSIZE: b, L1_SIZE: l1, L2_SIZE: l2, L1_ASSOC: a1, L2_ASSOC: a2, Victim_Cache_SIZE: v, trace_file: t},
								)
							}
						}
					}
				}
			}
		}
	}

	// Change Block Size
	// for _, b := range bs {
	// 	for _, t := range tf {
	// 		ps = append(ps, Parameters{BLOCKSIZE: b, L1_SIZE: 16384, L2_SIZE: 16384, L1_ASSOC: 4, L2_ASSOC: 4, Victim_Cache_SIZE: 2048, trace_file: t})
	// 	}
	// }
	//
	// // Change L1 Cache Size
	// for _, l1 := range l1s {
	// 	for _, t := range tf {
	// 		ps = append(ps, Parameters{BLOCKSIZE: 128, L1_SIZE: l1, L2_SIZE: 16384, L1_ASSOC: 4, L2_ASSOC: 4, Victim_Cache_SIZE: 2048, trace_file: t})
	// 	}
	// }
	//
	// // Change L2 Cache Size
	// for _, l2 := range l2s {
	// 	for _, t := range tf {
	// 		ps = append(ps, Parameters{BLOCKSIZE: 128, L1_SIZE: 16384, L2_SIZE: l2, L1_ASSOC: 4, L2_ASSOC: 4, Victim_Cache_SIZE: 2048, trace_file: t})
	// 	}
	// }
	//
	// // Change ass Assoc
	// for _, a1 := range l1a {
	// 	for _, t := range tf {
	// 		ps = append(ps, Parameters{BLOCKSIZE: 128, L1_SIZE: 16384, L2_SIZE: 16384, L1_ASSOC: a1, L2_ASSOC: 4, Victim_Cache_SIZE: 2048, trace_file: t})
	// 	}
	// }

	// Status := make(map[Parameters]string)
	Map := make(chan struct{}, MaxProc)
	results := make(chan string, 2*MaxProc)
	PC := make(chan Parameters, MaxProc)

	var wg sync.WaitGroup

	ctx, Exit := context.WithCancel(context.Background())
	defer Exit()

	for i := 0; i < MaxProc; i++ {
		wg.Add(1)
		go func(p chan Parameters) {
			defer wg.Done()
			runWrapper(ctx, p, results, Map)
		}(PC)
	}

	wg.Add(1)
	go func() {
		defer wg.Done()
		for _, p := range ps {
			PC <- p
		}
		Exit()
	}()

	go func() {
		if _, err := file.WriteString("BLOCKSIZE L1_SIZE L2_SIZE L1_ASSOC L2_ASSOC Victim_Cache_SIZE trace_file L1MissRate L2MissRate AAT\n"); err != nil {
			log.Printf("Error %v", err)
		}
		for result := range results {
			_, err := file.WriteString(result)
			if err != nil {
				log.Printf("Error writing to file: %v", err)
			}
		}
	}()

	total := len(ps)
	cur := 0
	bar := progressbar.Default(int64(total))

	f := func() {
		bar.Set(cur)
	}

	tick := time.NewTicker(1 * time.Second)

	go func(func()) {
		f()
		for {
			select {
			case <-tick.C:
				f()
			case <-Map:
				cur++
			}
		}
	}(f)

	wg.Wait()      // Wait for all training goroutines to finish
	close(results) // Close the results channel
}
