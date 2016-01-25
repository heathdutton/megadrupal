###############################################################################
##### Process Hooks
###############################################################################

Any custom process functionality can (rather than directly in template.php) be
placed in this process folder in a file named as such:

TEMPLATE_process_html() = process-html.inc
TEMPLATE_process_page() = process-page.inc
TEMPLATE_process_node() = process-node.inc
TEMPLATE_process_comment() = process-comment.inc
TEMPLATE_process_region() = process-region.inc
etc.

Inside of your process-HOOK.inc files, you can either directly dump the PHP code
as it would normally appear INSIDE of a process function, or you can optionally
(recommended) wrap the code in a custom hook for Alpha/Omega as such:

function THEMENAME_alpha_process_HOOK(&$vars) {
  // custom functionality here
}
