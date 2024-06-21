\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{ictex}[(Unofficial) Template for Coursework, Imperial College London]
% ICTex © 2023-2024 by Lancelot Liu is licensed under the MIT License
% Version: 1.9.5
% Email: lancylot2004@proton.me, lancelot.liu23@imperial.ac.uk

% \iff and \implies to single arrows.
\DeclareOption{singleArrows}{
    \renewcommand{\iff}{\longleftrightarrow}
    \renewcommand{\implies}{\longrightarrow}
}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}
\ProcessOptions\relax
\LoadClass[fleqn]{article}

% == Misc Packages == %
\RequirePackage{geometry}     % Fine-tuned page dimensions.
\RequirePackage{enumerate}    % Bullet-point, enumerated, and other lists.
\RequirePackage{amsmath}      % General mathematics improvements.
\RequirePackage{amssymb}      % Extra symbols.
\RequirePackage{tikz}         % Drawing.
\RequirePackage{xcolor}       % Colour everywhere!
\RequirePackage{multicol}     % Multicolumn formatting.
\RequirePackage{siunitx}      % Units.
\RequirePackage{enumitem}     % Better enumerate.
\RequirePackage{fontspec}     % Custom fonts.

\usetikzlibrary{shapes.geometric,fit} % Function diagrams.

% == Layout Packages == %
\RequirePackage{parskip}      % No indents.
\RequirePackage{fancyhdr}     % Headers and Footers
\RequirePackage{lastpage}     % Needed for total number of pages.
\RequirePackage{datetime}     % Needed for month in footer.

% == Figure Packages == %
\RequirePackage{subcaption}   % Subfigure captions.
\RequirePackage{caption}      % Figure captions.
\RequirePackage{wrapfig}      % Wrapped figures.
\RequirePackage{float}        % Figures in multicols.

% == Custom Environment Packages == %
\RequirePackage{mdframed}     % Needed for `problem` and `context`.
\RequirePackage{tcolorbox}    % Needed for `code`.
\RequirePackage{minted}       % Needed for `code`.

% == Computer Systems Packages == %
\RequirePackage{karnaugh-map} % Karnaugh Maps
    % \begin{karnaugh-map}[4][4][1][$AB$][$CD$]
    %     \minterms{0,1,2,3,5,8,9,10,13}
    %     \maxterms{4,6,7,11,12,14,15}
    %     \implicant{0}{2}
    %     \implicant{1}{9}
    %     \implicantcorner{0}{8}{2}{10}
    % \end{karnaugh-map}
\RequirePackage{circuitikz}   % Circuits
\RequirePackage{tikz-timing}  % Timing Diagrams

% == Mathematical Packages == %
\RequirePackage{braket}              % < | > notation.
\RequirePackage[linguistics]{forest} % Formation Trees.

% == Table Packages == %
\RequirePackage{array}        % Allows \newcolumntype
\RequirePackage{tabularx}     % Allows tables to be easily sized.
\RequirePackage{multirow}     % Allows multi-row spannig table entries.
\RequirePackage{colortbl}     % Required for a method of shading columns.

\newcolumntype{C}{>{$}c<{$}}  % Math-mode equivalents
\newcolumntype{L}{>{$}l<{$}}
\newcolumntype{R}{>{$}r<{$}}
\newcolumntype{H}{>{\columncolor[gray]{0.8}}l} % Highlighted "l" column type.

% == Options == %
\newcommand{\cName}{Lancelot Liu}
\newcommand{\cCourse}{COMP40000}
\newcommand{\cAssignName}{CW0}

\newcommand{\ictexConf}[3]{%
  \renewcommand{\cName}{#1}%
  \renewcommand{\cCourse}{#2}%
  \renewcommand{\cAssignName}{#3}%
}

% == Page and Header/Footer Setup == %
\geometry{
    a4paper,
    top=19mm,
    left=12.925mm,
    right=12.925mm,
    bottom=19mm
}

\setmonofont{JetBrains Mono}[
    Contextuals = Alternate,
    Ligatures = TeX,
    SizeFeatures={Size=8},
]

\pagestyle{fancy}
\fancyhf{}

\renewcommand{\headrulewidth}{0pt}
\newdateformat{monthyeardate}{\monthname[\THEMONTH], \THEYEAR}
\renewcommand{\footrulewidth}{0pt}

% == Commands: Color == %
\definecolor{cDarkBlue}{HTML}{00008B}
\definecolor{cLightBlue}{HTML}{F3F3FF}
\definecolor{cGreen}{rgb}{0,0.6,0}
\definecolor{cGray}{rgb}{0.5,0.5,0.5}
\definecolor{cPurple}{rgb}{0.58,0,0.82}

% == Commands: General == %

% Shaded box, titled, containing a problem and all its constituent parts.
\newenvironment{problem}[1]
    { \vspace{1pt} \begin{mdframed}[backgroundcolor=gray!20] {\vspace{4pt} \Large \textbf{#1}} \vspace{2pt} \\ }
    { \vspace{2pt} \end{mdframed} \vspace{1pt} }

% Shaded box, titled, providing context for a set of problems.
\newenvironment{context}[1]
    { \vspace{1pt} \begin{mdframed}[backgroundcolor=blue!20] {\vspace{4pt} \Large \textbf{#1}} \vspace{2pt} \\ }
    { \end{mdframed} \vspace{1pt} }

% Background-highlighted code box.
\tcbuselibrary{listings, minted, skins}
\tcbset{listing engine=minted}
\definecolor{inline}{RGB}{187,57,82}
\definecolor{bg}{RGB}{22,43,58}
\newtcblisting{code}{
  listing only, minted language=C, minted style=paraiso-dark,
  left=1pt, top=0pt, right=1pt, bottom=0pt,
  colback=bg, enhanced, frame hidden, minted options={
    fontsize=\small, tabsize=2, breaklines,
    autogobble, framesep=0mm, escapeinside=~~
  }
}

% Background-highlighted inline code box.
\newtcbox{\inline}{on line,
    arc=3pt, outer arc=3pt,
    colback=bg, colframe=white, colupper=white,
    boxsep=2pt, left=1pt, right=1pt, top=1.5pt, bottom=0.5pt,
    boxrule=0pt,
    before upper={\small \ttfamily},
    valign=center
}

% Text alignment tool.
\newenvironment{textAlign}[1][lX]
{ \vspace{1em} \tabularx{\linewidth}{#1} }
{ \endtabularx \vspace{1em} }

% Quotation box, left-aligned indent and highlight.
\newmdenv[
  innerleftmargin = 3pt,
  innerrightmargin = 5pt,
  innertopmargin = 5pt,
  innerbottommargin = 5pt,
  skipabove = \topsep,
  skipbelow = \topsep,
  linewidth = 2pt,
  linecolor = cDarkBlue,
  topline = false,
  bottomline = false,
  rightline = false,
  backgroundcolor = cLightBlue
]{formal}

% Partition box - bounded box with a title.
\usetikzlibrary{decorations.pathmorphing}
\tikzstyle{part} = [draw=black, fill=white, very thick,
  rectangle, rounded corners, inner sep=10pt, inner ysep=10pt]
\tikzstyle{partTitle} =[fill=black, text=white, font=\bfseries]

\newenvironment{partition}[2]
{
  \def\partTitle{#2}
  \begin{tikzpicture}
  \node [part] (part) \bgroup
    \begin{minipage}{#1\textwidth} \raggedright \bgroup
}{
    \egroup \end{minipage}
  \egroup;
  \node[partTitle, right=10pt] at (part.north west) {\partTitle};
  \end{tikzpicture}
}

% == Commands: Maths == %

% Show step in `align` environment. **Must be placed at ends of lines.**
\newcommand{\mstep}[1]{&& [#1]}
\newcommand{\step}[1]{\mstep{\text{#1}}}

% Use in $$ blocks in text to ensure proper display of fractions, sums, etc.
\newcommand{\ds}{\displaystyle}

% Logical Statement, Emphasised.
\newcommand{\state}[1]{{\color{teal}`#1'}}

% Angle bracket ordered sets.
\newcommand{\oset}[1]{\langle #1 \rangle}

% Derivatives
\newcommand{\ddx}{\dev{}{x}}
\newcommand{\dev}[2]{\frac{\text d#1}{\text d#2}}

% Matrices
\newcommand{\sqmat}[1]{\begin{bmatrix}#1\end{bmatrix}}
\newcommand{\augmat}[2]{\left[\begin{array}{#1}#2\end{array}\right]}

% Propositional Logic
\renewcommand{\tt}{\texttt{tt}} % Truth value.
\newcommand{\ff}{\texttt{ff}}   % False value.
\newcommand{\eval}[1]{|#1|_v}   % Evaluation function.

% Databases
\newcommand{\fk}{$\stackrel{\text{fk}}{\Rightarrow}$}
