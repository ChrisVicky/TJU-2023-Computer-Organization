import subprocess

def run_simulator(L1_SIZE, BLOCKSIZE=64, L2_SIZE=4096, L1_ASSOC=2, L2_ASSOC=4, Victim_Cache_SIZE=1024, trace_file="gcc_trace.txt"):
    cmd = ["./sim_cache", str(BLOCKSIZE), str(L1_SIZE), str(L2_SIZE), str(L1_ASSOC), str(L2_ASSOC), str(Victim_Cache_SIZE), trace_file]
    output = subprocess.check_output(cmd).decode('utf-8')
    return output

def parse_output(output):
    lines = output.split('\n')
    results = {}
    for line in lines:
        if "L1 miss rate:" in line:
            results['L1_miss_rate'] = float(line.split()[-1])
        elif "L2 miss rate:" in line:
            results['L2_miss_rate'] = float(line.split()[-1])
        elif "average access time:" in line:
            results['AAT'] = float(line.split()[-2])
    return results

# Define L1_SIZE values to simulate
L1_SIZES = [32, 64, 128, 256]

# Run simulations and collect results
results = {}
for L1_SIZE in L1_SIZES:
    output = run_simulator(L1_SIZE)
    parsed_results = parse_output(output)
    results[L1_SIZE] = parsed_results

print(results)
