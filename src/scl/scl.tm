<TeXmacs|1.0.7.10>

<style|generic>

<\body>
  AFEPack \<#89E3\>\<#4E00\>\<#4E2A\>\<#65F6\>\<#95F4\>\<#53D1\>\<#5C55\>\<#7684\>\<#FF0C\>\<#5B88\>\<#6052\>\<#578B\>\<#65B9\>\<#7A0B\>\<#6216\>\<#8005\>\<#65B9\>\<#7A0B\>\<#7EC4\>

  \<#7ED9\>\<#5B9A\>\<#4E8C\>\<#7EF4\>\<#533A\>\<#57DF\>\<#FF0C\>\<#533A\>\<#57DF\>\<#4E0A\>\<#5256\>\<#5206\>\<#4E09\>\<#89D2\>\<#5F62\>\<#7F51\>\<#683C\>
  Open Data Explorer

  \<#5B88\>\<#6052\>\<#578B\>\<#6570\>\<#503C\>\<#683C\>\<#5F0F\>

  \<#81EA\>\<#9002\>\<#5E94\>\<#7F51\>\<#683C\>\<#FF0C\>\<#5E76\>\<#884C\>\<#6C42\>\<#89E3\>\<#FF1A\>\<#8D1F\>\<#8F7D\>\<#5E73\>\<#8861\>

  <\equation>
    <frac|\<partial\>u|\<partial\>t>+<frac|\<partial\>|\<partial\>x>f<around*|(|u|)>+<frac|\<partial\>|\<partial\>y>g<around*|(|u|)>=0
  </equation>

  <math|<with|math-font|cal|T>:>\<#5355\>\<#5143\><math|e>\<#FF0C\>\<#8FB9\><math|s>\<#FF0C\>\<#70B9\><math|v>

  <\equation>
    <big-around|\<cup\>|e>=\<Omega\>\<nocomma\>,<space|1em>e<rsub|i><big-around|\<cap\>|>e<rsub|j>=\<emptyset\>
  </equation>

  \<#5B9A\>\<#4E49\>\<#89E3\>\<#7A7A\>\<#95F4\><math|V<rsub|h>=<around*|{|v<rsub|h><around*|(|x,y|)>;
  v<rsub|h>=v<rsub|i>\<nocomma\>,if <around*|(|x,y|)>\<in\>e<rsub|i>|}>>

  <\equation>
    <big-around|\<int\>|<rsub|\<Omega\>><frac|\<partial\>u|\<partial\>t>v
    d<wide|x|\<vect\>>+<big-around|\<int\>|<rsub|\<Omega\>><frac|\<partial\>f|\<partial\>x>>v
    d<wide|x|\<vect\>>+<big-around|\<int\>|<rsub|W><frac|\<partial\>g|\<partial\>y>v
    d<wide|x|\<vect\>>=0>>
  </equation>

  <\equation>
    <big-around|\<int\>|<rsub|e><frac|\<partial\>u<rsub|i>|\<partial\>t>d><wide|x|\<vect\>>+<big-around|\<int\>|<rsub|s<rsub|i
    j>\<in\>\<partial\>e><around*|[|<tabular|<tformat|<table|<row|<cell|F<around*|(|u<rsub|i>,u<rsub|j>|)>>>|<row|<cell|G<around*|(|u<rsub|i>,u<rsub|j>|)>>>>>>|]>\<cdot\><wide|n|\<vect\>>d
    s>=0
  </equation>

  <\equation>
    <frac|u<rsub|i><rsup|n+1>-u<rsub|i><rsup|n>|\<Delta\>t><around*|\||e|\|>+<big-around|\<sum\>|<rsub|k=1><rsup|N>>F<around*|(|u<rsub|i><rsup|n>,u<rsub|j><rsup|n>|)><around*|(|x<rsub|k>|)>\<omega\><rsub|k><around*|\||s<rsub|i
    j>|\|>=0
  </equation>

  Local-Lax-Friedrich\<#6570\>\<#503C\>\<#901A\>\<#91CF\>\<#FF1A\>

  <\equation>
    F<around*|(|u<rsub|i><rsup|n>,u<rsub|j><rsup|n>|)>=<frac|A|2>-\<lambda\><around|(|u<rsub|j><rsup|n>-u<rsub|i><rsup|n>|)>
  </equation>

  <\equation>
    A=<around*|[|<tabular|<tformat|<table|<row|<cell|f<around*|(|u<rsub|i>|)>+f<around*|(|u<rsub|j>|)>>>|<row|<cell|g<around*|(|u<rsub|i>|)>+g<around*|(|u<rsub|j>|)>>>>>>|]>\<cdot\><wide|n|\<vect\>>
  </equation>

  <\equation>
    \<lambda\>=max<around*|{|\<lambda\><rsub|i>,\<lambda\><rsub|j>|}>
  </equation>

  <\equation>
    \<lambda\><rsub|i>=<around*|\||<frac|\<mathd\><around|(|f<around*|(|u<rsub|i>|)>n<rsub|x>+g<around*|(|u<rsub|i>|)>n<rsub|y>|)>|\<mathd\>u>|\|>
  </equation>

  CFL\<#6761\>\<#4EF6\>\<#FF1A\>CFL\<#6570\>(CFL\<less\>1)

  <\equation>
    \<Delta\>t=min<rsub|e>\<Delta\>t<rsub|e>,<space|1em>\<Delta\>t<rsub|e>=h<rsub|e>
  </equation>

  \<#81EA\>\<#9002\>\<#5E94\>\<#6307\>\<#793A\>\<#5B50\>

  <\equation>
    <around*|\||u-u<rsub|h>|\|><rsub|e>\<approx\><big-around|\<sum\>|<rsub|l\<in\>\<partial\>e><around*|\||u<rsub|i>-u<rsub|j>|\|>
    <around*|\||l|\|><rsup|2>>
  </equation>

  <\equation>
    <frac|\<partial\>|\<partial\>t><around*|[|<tabular|<tformat|<cwith|1|1|1|1|cell-halign|c>|<cwith|2|2|1|1|cell-halign|c>|<cwith|3|3|1|1|cell-halign|c>|<cwith|4|4|1|1|cell-halign|c>|<table|<row|<cell|\<rho\>>>|<row|<cell|\<rho\>u>>|<row|<cell|\<rho\>v>>|<row|<cell|E>>>>>|]>+<frac|\<partial\>|\<partial\>x><around*|[|<tabular|<tformat|<cwith|1|1|1|1|cell-halign|c>|<cwith|3|3|1|1|cell-halign|c>|<table|<row|<cell|\<rho\>u>>|<row|<cell|\<rho\>u<rsup|2>+p>>|<row|<cell|\<rho\>u
    v>>|<row|<cell|u<around|(|E+p|)>>>>>>|]>+<frac|\<partial\>|\<partial\>y><around*|[|<tabular|<tformat|<cwith|1|1|1|1|cell-halign|c>|<cwith|2|2|1|1|cell-halign|c>|<cwith|3|3|1|1|cell-halign|c>|<cwith|4|4|1|1|cell-halign|c>|<table|<row|<cell|\<rho\>v>>|<row|<cell|\<rho\>u
    v>>|<row|<cell|\<rho\>v<rsup|2>+p>>|<row|<cell|v<around*|(|E+p|)>>>>>>|]>=0
  </equation>

  <\equation>
    e=<frac|p|\<gamma\>-1>,E=\<rho\>e+<frac|1|2>\<rho\><around|(|u<rsup|2>+v<rsup|2>|)>,\<gamma\>=<frac|n+2|n>
  </equation>

  <\equation>
    \<lambda\>=V,V\<pm\>c,c=<sqrt|<frac|\<gamma\>p|\<rho\>>>
  </equation>

  \;

  \;

  \;

  \;
</body>

<\initial>
  <\collection>
    <associate|font|sys-chinese>
    <associate|language|chinese>
    <associate|sfactor|3>
  </collection>
</initial>