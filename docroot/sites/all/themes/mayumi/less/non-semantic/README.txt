1. Drop this folder "non-semantic" to your less folder. It should look like this:
     less/main.css.less
     less/custom
     less/non-semantic
2. Open your main.css.less file and add this at the bottom:
     @import url("non-semantic/non-semantic-all.css.less");

You may then use presentation classes on your markup.
