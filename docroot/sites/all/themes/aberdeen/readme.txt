
Aberdeen is a fresh design that balances simplicity, soft, neutral background colors, plenty of whitespace and big nice typography.

List of features

* Standards-compliant XHTML 1.0 Strict and CSS.
* Liquid CSS layout (tableless) - the whole layout increases or decreases proportionally as dimensions are specified in ems. Try changing the font size to see this working.
* Supports one, two and three columns.
* Cross-browser compatible.
* Cute icons (all GPL).
* Coded with SEO in mind, the order of the columns are 2dn, 3rd and 1st - usually the 1st is intended for navigation, the other two for content, Google likes that.
* Slinding doors tabs for primary links that blend with main content area.

Notes
 * If you want to place a menu in the header region uncomment out the CSS in the style.css file

1. Aberdeen Layout

If all columns are enabled, the visual layout is divided in four equal columns, with the center one taking twice the width of the sidebars.

For SEO the order in the HTML is re-arranged by the stylesheet.

Note the HTML structure - if one column changes its width it affects the other ones. 

|------------------------------#main------------------------------------|
|-------------------#content---------------------||----#sidebar_left----|
|-----------#center--------||---#sidebar_right---|

If you want to change the width of the sidebars go to the layout section in the style.css file.

These are the original dimensions
/*****************************************************************/
#main {
  width: 68.5em;
  ...
}
#sidebar-left {
  width: 16em;
  ...
}
#sidebar-right {
  width: 16em;
  ...
}
#center {
  width: 100%; /* if #center is the only column */
  ...
}
body.sidebar-left #center, body.sidebar-right #center { /* if #center shares full width with one other column */
  width: 51em;
}
body.sidebars #center { /* if #center, #sidebar-left and #sidebar-right are all present */
  width: 33.4em;
}
body.sidebars #content {
  width: 51em;
}
/*****************************************************************/


If we want sidebar_left and sidebar_right to be 12ems instead of 16em then

/*****************************************************************/
#main {
  width: 68.5em;
  .../* more styles here */
}
#sidebar-left {
  width: 12em;
  ...
}
#sidebar-right {
  width: 12em;
  ...
}
#center {
  width: 100%;
  ...
}
body.sidebar-left #center, body.sidebar-right #center {
  width: 55em; /* 51em + 4em gained from the sidebar present */
}
body.sidebars #center {
  width: 41.4em; /*33.4em + 4em gained from sidebar_left + 4em gained from sidebar_right*/
}
body.sidebars #content {
  width: 55em; /*51em + 4em gained from sidebar_right*/
}
/*****************************************************************/
