// ----------------------------------------------------------------------------
// markItUp!
// ----------------------------------------------------------------------------
// Copyright (C) 2008 Jay Salvat
// http://markitup.jaysalvat.com/
// ----------------------------------------------------------------------------
// Html tags
// http://en.wikipedia.org/wiki/html
// ----------------------------------------------------------------------------
// Basic set. Feel free to add more tags
// ----------------------------------------------------------------------------
myLaTeXSettings = {	
	onShiftEnter:  	{keepDefault:false, replaceWith:'<br />\n'},
	onCtrlEnter:  	{keepDefault:false, openWith:'\n<p>', closeWith:'</p>'},
	onTab:    		{keepDefault:false, replaceWith:'    '},
	markupSet:  [ 	
		{name:'LaTeX', className:'latex', key:'T', replaceWith:'\\LaTeX' },

		{ name:'Lowercase Greek Letters', className:'alpha', dropMenu: [
    			{ className:'alpha', replaceWith:'\\alpha'}, 
			{ className:'beta', replaceWith:'\\beta'},
			{ className:'gamma', replaceWith:'\\gamma'},
			{ className:'delta', replaceWith:'\\delta'},
			{ className:'epsilon', replaceWith:'\\epsilon'},
			{ className:'zeta', replaceWith:'\\zeta'},
			{ className:'eta', replaceWith:'\\eta'},
			{ className:'theta', replaceWith:'\\theta'},
			{ className:'iota', replaceWith:'\\iota'},
			{ className:'kappa', replaceWith:'\\kappa'},
  			{ className:'lambda', replaceWith:'\\lambda'},
			{ className:'mu', replaceWith:'\\mu'},
			{ className:'nu', replaceWith:'\\nu'},
			{ className:'xi', replaceWith:'\\xi'},
			{ className:'omicron', replaceWith:'o'},
			{ className:'pi', replaceWith:'\\pi'},
			{ className:'rho', replaceWith:'\\rho'},
			{ className:'sigma', replaceWith:'\\sigma'},
			{ className:'tau', replaceWith:'\\tau'},
			{ className:'upsilon', replaceWith:'\\upsilon'},
			{ className:'phi', replaceWith:'\\phi'},
			{ className:'chi', replaceWith:'\\chi'},
			{ className:'psi', replaceWith:'\\psi'},
			{ className:'omega', replaceWith:'\\omega'},
			{ className:'aleph', replaceWith:'\\aleph'}
			]
		},
		{ name:'Capital Greek Letters', className:'cGamma', dropMenu: [
			{ className:'cGamma', replaceWith:'\\Gamma'},
			{ className:'cDelta', replaceWith:'\\Delta'},
			{ className:'cTheta', replaceWith:'\\Theta'},
			{ className:'cLambda', replaceWith:'\\Lambda'},
			{ className:'cXi', replaceWith:'\\Xi'},
			{ className:'cPi', replaceWith:'\\Pi'},
			{ className:'cSigma', replaceWith:'\\Sigma'},
			{ className:'cUpsilon', replaceWith:'\\Upsilon'},
			{ className:'cPhi', replaceWith:'\\Phi'},
			{ className:'cPsi', replaceWith:'\\Psi'},
			{ className:'cOmega', replaceWith:'\\Omega'}
			]
		},
		{ name:'Calculus', className:'int', dropMenu: [
                        { className:'int', replaceWith:'\\int'},
			{ className:'iint', replaceWith:'\\iint'},
			{ className:'iiint', replaceWith:'\\iiint'},
			{ className:'x-prime', replaceWith:'x\''},
			{ className:'x-prime-prime', replaceWith:'x\'\''},
			{ className:'x-triple-prime', replaceWith:'x\'\'\''},
			{ className:'int_lim', replaceWith:'\\int_{0}^{1}'},
			{ className:'ddx', replaceWith:'\\frac {d}{dx}'},
			{ className:'partial', replaceWith:'\\partial'},
			{ className:'dydx', replaceWith:'\\frac {dy}{dx}'},
			{ className:'cSigma2', replaceWith:'\\sum_{i=0}^{\\infty}f(x)'},
			{ className:'cPi2', replaceWith:'\\prod_{i=0}^{\\infty}f(x)'},
			{ className:'limit', replaceWith:'\\lim\\limits_\{x\\rightarrow\\infty\}f(x)'},
			{ className:'permutation', replaceWith:'_{n}\\textrm{P}_{k}'},
			{ className:'combination', replaceWith:'_{n}\\textrm{C}_{k}'}
			]
		},
		{ name:'Exponents, Subscripts, and Accents', className:'xtothey', dropMenu: [
                        { className:'xtothey', replaceWith:'x^{y}'},
			{ className:'xsuby', replaceWith:'x_{y}'},
			{ className:'xdoubleyz', replaceWith:'x_{y}^{z}'},
			{ className:'frac', replaceWith:'\\frac xy'},
			{ className:'sqrt', openWith:'\\sqrt[]{', closeWith:'}'},
			{ className:'bar', openWith:'\\bar{', closeWith:'}'},
			{ className:'dot', replaceWith: '\\dot x'},
			{ className:'ddot', replaceWith: '\\ddot x'},
			{ className:'hat', replaceWith: '\\hat{x}'},
			{ className:'grave', replaceWith: '\\grave{x}'},
			{ className:'acute', replaceWith: '\\acute{x}'},
			{ className:'check', replaceWith: '\\check{x}'}
			]
		},
		{ name:'Parenthesis', className:'brace', dropMenu: [
                        { className:'brace', openWith:'\\left{', closeWith:'\\right}'},
                        { className:'parenthesis', openWith:'\\left(', closeWith:'\\right)'},
			{ className:'abs_val', openWith:'\\left|', closeWith:'\\right|'},
			{ className:'langle-rangle', openWith:'\\left\\langle', closeWith:'\\right\\rangle'}
			]
                },
		{ name:'Arrows, Floors, and Ceilings', className:'crightarrow', dropMenu: [
                        { className:'crightarrow', replaceWith:'\\Rightarrow'},
			{ className:'cleftarrow', replaceWith:'\\Leftarrow'},
			{ className:'rightarrow', replaceWith:'\\rightarrow'},
			{ className:'leftarrow', replaceWith:'\\leftarrow'},
			{ className:'overset_leftarrow', openWith:'\\overset{', closeWith:'}{\\leftarrow}'},
			{ className:'overset-arrows', replaceWith: '\\overset{\\leftrightarrow}{x}'},
			{ className:'overset_rightarrow', openWith:'\\overset{', closeWith:'}{\\rightarrow}'},
			{ className:'overleftarrow', openWith:'\\vec{', closeWith:'}'},
			{ className:'overrightarrow', openWith:'\\vec{', closeWith:'}'},
			{ className:'sDownarrow', replaceWith:'\\Downarrow'},
			{ className:'cUparrow', replaceWith:'\\Uparrow'},
			{ className:'Updownarrow', replaceWith:'\\Updownarrow'},
			{ className:'uparrow', replaceWith:'\\uparrow'},
			{ className:'downarrow', replaceWith:'\\downarrow'},
			{ className:'supdownarrow', replaceWith:'\\updownarrow'},
			{ className:'lceil', replaceWith:'\\lceil'},
			{ className:'rceil', replaceWith:'\\rceil'},
			{ className:'lfloor', replaceWith:'\\lfloor'},
			{ className:'rfloor', replaceWith:'\\rfloor'},
			{ className:'langle', replaceWith:'\\langle'},
			{ className:'rangle', replaceWith:'\\rangle'},
			]
                },
		{ name:'Matrices and Structures', className:'piecewise', dropMenu: [
                        { className:'piecewise', replaceWith:'\\begin{cases} & \\text{ if },\\, x= \\\\ & \\text{ if },\\, x= \\\\ & \\text{ if },\\, x= \\end{cases}'},
			{ className:'eqnarray', replaceWith:'\\begin{eqnarray} &=& \\\\ &=& \\\\ &=& \\end{eqnarray}'},
			{ className:'bmatrix', replaceWith:''},
			{ className:'pmatrix', replaceWith:''},
			{ className:'ddots', replaceWith:'\\ddots'},
			{ className:'vdots', replaceWith:'\\vdots'},
			{ className:'dots', replaceWith:'\\dots'},	
                        ]			
                },
		{ name:'Symbols', className:'leq', dropMenu: [
			{ className:'less-than', replaceWith:'<'},
			{ className:'greater-than', replaceWith:'>'},
                        { className:'leq', replaceWith:'\\leq'},
                        { className:'geq', replaceWith:'\\geq'},
			{ className:'neq', replaceWith:'\\neq'},
			{ className:'pm', replaceWith:'\\pm'},
			{ className:'prec', replaceWith:'\\prec'},
			{ className:'preceq', replaceWith:'\\preceq'},
			{ className:'succ', replaceWith:'\\succ'},
			{ className:'succeq', replaceWith:'\\succeq'},
			{ className:'equiv', replaceWith:'\\equiv'},
			{ className:'approx', replaceWith:'\\approx'},
			{ className:'pm', replaceWith:'\\pm'},
			{ className:'div', replaceWith:'\\div'},
			{ className:'times', replaceWith:'\\times'},
			{ className:'cdot', replaceWith:'\\cdot'},
			{ className:'asterisk', replaceWith:'*'},
			{ className:'otimes', replaceWith:'\\otimes'},
			{ className:'oplus', replaceWith:'\\oplus'},
			{ className:'infty', replaceWith:'\\infty'},
			{ className:'cap', replaceWith:'\\cap'},
			{ className:'cup', replaceWith:'\\cup'},
			{ className:'bigcap', replaceWith:'\\bigcap_{}^{}'},
			{ className:'bigcup', replaceWith:'\\bigcup_{}^{}'},
			{ className:'subset', replaceWith:'\\subset'},
			{ className:'subseteq', replaceWith:'\\subseteq'},
			{ className:'in', replaceWith:'\\in'},
			{ className:'notin', replaceWith:'\\notin'},
			{ className:'sim', replaceWith:'\\sim'},
			{ className:'notsubset', replaceWith:'\\cancel{subset}'},
			{ className:'nsubseteq', replaceWith:'\\cancel{\\subseteq}'},
			{ className:'exists', replaceWith:'\\exists'},
			{ className:'forall', replaceWith:'\\forall'},
			{ className:'dots', replaceWith:'\\dots'},
                        ]
                },
		{ name:'Number Systems', className:'mathbbn', dropMenu: [
                        { className:'mathbbn', replaceWith:'\\mathbb{N}'},
                        { className:'mathbbz', replaceWith:'\\mathbb{Z}'},
                        { className:'mathbbq', replaceWith:'\\mathbb{Q}'},
			{ className:'mathbbr', replaceWith:'\\mathbb{R}'},
			{ className:'mathbbc', replaceWith:'\\mathbb{C}'}
                        ]
                },

		{name:'Fonts and Marks', className:'bold', dropMenu: [
			{className:'bold', key:'I', openWith:'{\\bf ', closeWith:'}'  },
                        {className:'stoke', key:'I', openWith:'{\\it ', closeWith:'}'  },
                        {className:'stroke', key:'S', openWith:'\\sout{', closeWith:'}' },
			{ className:'cancel', openWith:'\\cancel{', closeWith:'}'},
			{className:'underline', openWith:'\\underline{', closeWith:'}' },
			{className:'mathscrf', openWith:'\\scr{', closeWith:'}' }
                        ]
                },

		{ name:'Spacing', className:'small_space', dropMenu: [
			{ className:'negative_space', replaceWith:'\\!'},
                        { className:'small_space', replaceWith:'\\,'},
                        { className:'medium_space', replaceWith:'\\:'},
                        { className:'big_space', replaceWith:'\\;'}
                        ]
                }
	]
}

