api = 2
core = 7.x

; SVG Social Web Icons by Fairhead Creative (http://fairheadcreative.com/)
libraries[webicons][download][type] = "get"
libraries[webicons][download][url] = "https://github.com/adamfairhead/webicons/archive/master.zip"
libraries[webicons][directory_name] = "webicons"
libraries[webicons][destination] = libraries

; SVG Icons requires Modernizr
projects[modernizr][subdir] = contrib
libraries[modernizr][download][type] = file
libraries[modernizr][download][url] = http://modernizr.com/downloads/modernizr-latest.js
libraries[modernizr][download][filename] = modernizr.min.js
libraries[modernizr][destination] = libraries
