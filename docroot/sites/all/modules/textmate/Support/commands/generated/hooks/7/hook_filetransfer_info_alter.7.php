/**
 * Implements hook_filetransfer_info_alter().
 */
function <?php print $basename; ?>_filetransfer_info_alter(&\$filetransfer_info) {
  ${1:if (variable_get('paranoia', FALSE)) {
    // Remove the FTP option entirely.
    unset(\$filetransfer_info['ftp']);
    // Make sure the SSH option is listed first.
    \$filetransfer_info['ssh']['weight'] = -10;
  \}}
}

$2