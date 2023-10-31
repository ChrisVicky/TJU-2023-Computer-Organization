\ProvidesClass{tjureport}
\LoadClass{ctexart}
\RequirePackage{xeCJK}
\RequirePackage{CJKnumb}
\RequirePackage{graphicx}
\RequirePackage{geometry}
\RequirePackage{titlesec}
\RequirePackage{fancyhdr}
\RequirePackage{enumitem}
\RequirePackage{listings}
\RequirePackage{xcolor}
\RequirePackage{array}
\RequirePackage{mathtools}
\RequirePackage{float}
\RequirePackage{indentfirst}
\RequirePackage{zhnumber}
\RequirePackage{booktabs}
\RequirePackage[caption=false]{subfig}
\RequirePackage{threeparttable}

\newcommand{\yihao}{\fontsize{26pt}{26pt}\selectfont}       % 一号, 单倍行距
\newcommand{\xiaoyi}{\fontsize{24pt}{24pt}\selectfont}      % 小一, 单倍行距
\newcommand{\erhao}{\fontsize{22pt}{1.25\baselineskip}\selectfont}       % 二号, 1.25倍行距
\newcommand{\xiaoer}{\fontsize{18pt}{18pt}\selectfont}      % 小二, 单倍行距
\newcommand{\sanhao}{\fontsize{16pt}{16pt}\selectfont}      % 三号, 单倍行距
\newcommand{\xiaosan}{\fontsize{15pt}{15pt}\selectfont}     % 小三, 单倍行距
\newcommand{\sihao}{\fontsize{14pt}{14pt}\selectfont}       % 四号, 单倍行距
\newcommand{\xiaosi}{\fontsize{12pt}{12pt}\selectfont}      % 小四, 单倍行距
\newcommand{\wuhao}{\fontsize{10.5pt}{10.5pt}\selectfont}   % 五号, 单倍行距
\newcommand{\xiaowu}{\fontsize{9pt}{9pt}\selectfont}        % 小五, 单倍行距

\linespread{1.2}
\geometry{top=1in,bottom=1in,left=1.5in,right=1.5in}
\setcounter{secnumdepth}{3}
% \setlist[enumerate,1]{label=(\arabic*)}
% \setlist[enumerate,2]{label=\alph*.}

\newcommand{\figuremacro}[3]{
  \begin{figure}[!htbp]
    \centering
    \includegraphics[width=#3\columnwidth]{#1}
    \caption{#2}
    \label{fig:#1}
  \end{figure}
}

% \figuremacro{fig.png}{Name of the Figure}{1.0}

\newcommand{\subfmacro}[3]{
    \subfloat[#2]{
        \includegraphics[width=#3\textwidth]{#1}
    }
}
% \subfmacro{fig.png}{Caption}{1.0}

%
% \setCJKmainfont[
%     Path = fonts/ ,
%     Extension = .ttf ,
%     BoldFont = SimHei ,
%     ItalicFont = simkai ,
% ]{SimSun}

\titleformat*{\section}{\large\bfseries}
% \titleformat*{\subsection}{\normalsize\bfseries}
% \titleformat*{\subsubsection}{\normalsize}
\graphicspath{{figures/}{figure/}{pictures/}%
{picture/}{pic/}{pics/}{image/}{images/}}

\titlespacing{\subsection}{1em}{*4}{*1.5}
\titlespacing{\subsubsection}{1em}{*4}{*1.5}

% 代码格式和颜色定义
\definecolor{dkgreen}{rgb}{0,0.6,0}
\definecolor{gray}{rgb}{0.5,0.5,0.5}
\definecolor{comment}{rgb}{0.56,0.64,0.68}
\lstset{
    frame=tb,
    aboveskip=3mm,
    belowskip=3mm,
    xleftmargin=2em,
    xrightmargin=2em,
    showstringspaces=false,
    columns=flexible,
    framerule=1pt,
    rulecolor=\color{gray!35},
    backgroundcolor=\color{gray!5},
    basicstyle={\small\ttfamily},
    numbers=none,
    numberstyle=\tiny\color{gray},
    keywordstyle=\color{blue},
    commentstyle=\color{comment},
    stringstyle=\color{dkgreen},
    breaklines=true,
    breakatwhitespace=true,
    tabsize=2,
}

% 信息
\newcommand\major[1]{\def\@major{#1}}
\newcommand\class[1]{\def\@class{#1}}
\newcommand\maintitle[1]{\def\@maintitle{#1}}
\newcommand\subtitle[1]{\def\@subtitle{#1}}
\newcommand\myheader[1]{\def\@myheader{#1}}
\newcommand\name[1]{\def\@name{#1}}
\newcommand\stuid[1]{\def\@stuid{#1}}
\newcommand\college[1]{\def\@college{#1}}
\renewcommand\date[1]{\def\@date{#1}}
\newcommand\lab[1]{\def\@lab{#1}}
\newcommand\course[1]{\def\@course{#1}}
\newcommand\instructor[1]{\def\@instructor{#1}}
\newcommand\grades[1]{\def\@grades{#1}}
\newcommand\expname[1]{\def\@expname{#1}}
\newcommand\exptype[1]{\def\@exptype{#1}}
\newcommand\partner[1]{\def\@partner{#1}}

% 页眉
\newcommand{\startheader}{
\pagestyle{fancy}
\fancyhead{} 
\fancyhead[C]{\@myheader}
}

% 下划线定义
\def\tjuu[#1]#2{
    \underline{\hbox to #1{\hfil#2\hfil}}}

% 首页头部
\newcommand{\makeheader}{
    \begin{figure}[h]
        \begin{minipage}{0.7\linewidth}
            \centerline{\includegraphics[width=\linewidth]{head}}
        \end{minipage}
        \hfill
        \begin{minipage}{.3\linewidth}
            \raggedleft
            \begin{tabular*}{.8\linewidth}{ll}
                专业： & \tjuu[70pt]\@major   \\
                姓名： & \tjuu[70pt]\@name    \\
                学号： & \tjuu[70pt]\@stuid   \\
                日期： & \tjuu[70pt]\@date \\
                地点： & \tjuu[70pt]\@lab
            \end{tabular*}
        \end{minipage}
    \end{figure}

    \begin{table}[!htbp]
        \centering
        \begin{tabular*}{\linewidth}{llllll}
            课程名称： & \tjuu[150pt]\@course   & 指导老师： & \tjuu[50pt]\@instructor   & 成绩：       &  \tjuu[50pt]\@grades \\
            实验名称： & \tjuu[150pt]\@expname & 实验类型： &\tjuu[50pt]\@exptype & 同组学生姓名：& \tjuu[50pt]\@partner
        \end{tabular*}
    \end{table}
    \thispagestyle{empty}

}

% 封面
\newcommand{\makecover}{
    \begin{center}
        
      \vspace{1.5cm}
        \begin{center}
            \includegraphics[width=0.4\textwidth]{figures/tjuname.eps}
        \end{center}
        % \bfseries\large{\@college}\\
        \vspace{1cm}
        \bfseries\huge{\@title}\\
        \vspace{0.6cm}
        \bfseries\xiaoer{\@subtitle}\\
        \vspace{1.5cm}

        \begin{figure}[h]
            \centering
            \includegraphics[width=0.36\textwidth]{figures/tjulogo.eps}
        \end{figure}
        
        \newlength{\@title@width}
        \setlength{\@title@width}{7cm}
        \Large
        % 题目：\underline{\makebox[\@title@width][c]{\@expname}}\\
        \vspace{1cm}
        \begin{center}
            \linespread{2}
            \centering
            \renewcommand\arraystretch{1.6}
            \setlength{\@title@width}{5cm}
            \begin{tabular}{lc}
                % \bfseries{课程名称}    & \underline{\makebox[\@title@width][c]{\@course     }}   \\
                \bfseries{学\qquad 院} & \underline{\makebox[\@title@width][c]{\@college  }}    \\ 
                \bfseries{专\qquad 业} & \underline{\makebox[\@title@width][c]{\@major    }}   \\ 
                % \bfseries{班\qquad 级} & \underline{\makebox[\@title@width][c]{\@class}}   \\  
                \bfseries{学\qquad 号} & \underline{\makebox[\@title@width][c]{\@stuid    }}   \\
                \bfseries{姓\qquad 名} & \underline{\makebox[\@title@width][c]{\@name     }}   \\  
                % \bfseries{指导老师}    & \underline{\makebox[\@title@width][c]{\@instructor}}   \\ 
            \end{tabular}
        \end{center}
        \vspace{1cm}
        \Large{\@date}
    \end{center}
    \thispagestyle{empty}
    \newpage
}
