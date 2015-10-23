Prism is a new lightweight, extensible syntax highlighter, built with modern
web standards in mind. It’s a spin-off from Dabblet [1] and is tested there daily
by thousands.

This module will provide proper integration with the 3rd-party library, a
simple API for developers, an input filter, and expose all options of this
library via standard Drupal settings forms.

To use this module enable the syntax filter in one of the text format settings pages e.g.
admin/config/content/formats/full_html

The library path must resemble something like;
sites/all/libraries/prism/prism.js
sites/all/libraries/prism/prism.css

Once enabled your code simply needs to be wrapped in the correct syntax & inserted
into any text area. Here is an example using css highlighting.
[prism:css]
a { 
	color: #7BC673; 
}
p a { 
	color: #22272A; 
} 
p a:hover { 
	color: #7BC673; 
}
[/prism:css]

Read more about this library at Prismjs.com [2], also, read this blog post by
Lea Verou [3] introducing it.

[1] http://dabblet.com/
[2] http://prismjs.com/
[3] http://lea.verou.me/2012/07/introducing-prism-an-awesome-new-syntax-highlighter/
