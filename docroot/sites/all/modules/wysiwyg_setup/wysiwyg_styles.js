/*
 * Inspired by http://drupal.org/node/746524#comment-3351158
 */

CKEDITOR.addStylesSet('wysiwyg_setup_styles',
[
    // Block Styles
    { name : 'Title', element : 'h2', styles : { 'color' : '#12637A', 'font-size' : '18px' } },
    { name : 'Sub-title' , element : 'h3', styles : { 'color' : '#12637A', 'font-size' : '15px' , 'font-weight' : 'bold' } },
    { name : 'Left-to-Right' , element : 'p', styles : { 'direction' : 'ltr', 'text-align' : 'left'} },
    { name : 'Right-to-Left' , element : 'p', styles : { 'direction' : 'rtl', 'text-align' : 'right'} }
]); 
