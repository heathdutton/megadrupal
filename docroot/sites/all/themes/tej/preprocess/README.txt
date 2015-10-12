###############################################################################
##### Preprocess Hooks
###############################################################################

Any custom preprocess functionality can (rather than directly in template.php)
be placed in this preprocess folder in a file named as such:

TEMPLATE_preprocess_html() = preprocess-html.inc
TEMPLATE_preprocess_page() = preprocess-page.inc
TEMPLATE_preprocess_node() = preprocess-node.inc
TEMPLATE_preprocess_comment() = preprocess-comment.inc
TEMPLATE_preprocess_region() = preprocess-region.inc
etc.

Inside of your preprocess-HOOK.inc files, you can either directly dump the PHP
code as it would normally appear INSIDE of a preprocess function, or you can
optionally (recommended) wrap the code in a custom hook for Alpha/Omega as:

function THEMENAME_alpha_preprocess_HOOK(&$vars) {
  // custom functionality here
}
