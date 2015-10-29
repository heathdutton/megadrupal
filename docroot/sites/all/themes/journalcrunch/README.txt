1. Configure default Image styles (Configuration » Media » Image styles - admin/config/media/image-styles)
- Large: Override defaults - Scale and crop 593x348
- Medium: Override defaults - Scale and crop 430x290

2. Medium Image style
With Medium Image style, we ensure that the images attached to "Sticky at top of lists" Articles will not exceed the desired dimensions. Of course, Medium Image style satisfies only "Sticky at top of lists" Articles.

For the images of no "Sticky at top of lists" Articles (but with Medium Image style), is implemented a CSS based* resizing inside style.css file,

Line 88-90*
#content .node-front .nodeInner img { padding:0; width:255px; overflow:hidden; }
.node-front div.field-type-image { display:block; overflow:hidden; height:120px; }

3. Edit Social Media links directly inside page.tpl.php

4. You can find all sources inside JournalCrunch's misc folder