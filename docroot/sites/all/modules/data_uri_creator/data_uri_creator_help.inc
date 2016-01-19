<?php
/**
 * @file
 * Contains the help text for the Data URI Creator module.
 */

/**
 * Get the general help-text for the module.
 */
function _data_uri_creator_get_general_help() {
  $url_options = array('attributes' => array('target' => '_blank'));
  $help = '<h3 id="data-uri-creator-about">' . t('About') . '</h3>';
  $help .= '<p>';
  $help .= t(
    'The Data URI Creator is a lightweight standalone module that implements a !utility_page for manual generation of !data_uris (see !rfc2397). The tool can be particularly useful when you craft HTML pages, CSS or JSON data by hand and want to embed images directly in the content, as opposed to indirectly through a link to a separate image file.',
    array(
      '!data_uris' => l(t('Data URIs'), 'http://en.wikipedia.org/wiki/Data_URI_scheme', $url_options),
      '!rfc2397' => l(t('RFC 2397: The "data" URL scheme'), 'http://tools.ietf.org/html/rfc2397', $url_options),
      '!utility_page' => l(t('utility page'), DATA_URI_CREATOR_PAGE_PATH, $url_options),
    )
  );
  $help .= '</p><p>';
  $help .= t('The module also provides a lean API that can be used programmatically to detect, encode and decode Data URIs (through the <code>DataUriCreator</code> class). Therefore, other modules that work with Data URIs can reference the Data URI Creator module as a common API to perform encoding and decoding of such URIs.');
  $help .= '</p>';

  $help .= '<h3 id="data-uri-creator-configuration">' . t('Configuration') . '</h3>';
  $help .= '<p>';
  $help .= t('No further configuration is required after the module is installed and enabled, unless the utility page should be made available also to anonymous or other specific users.');
  $help .= '</p><p>';
  $help .= t(
    'To achieve this, one may optionally tweak the default settings through the <em>Configuration</em> &raquo; <em>!settings</em> page:',
    array(
      '!settings' => l(t('Data URI Creator settings'), DATA_URI_CREATOR_CONFIG_PATH, $url_options),
    )
  );
  $help .= '</p>';
  $help .= '<ul><li>';
  $help .= t('Specify the maximum allowed byte-size for data uploads, and');
  $help .= '</li><li>';
  $help .= t(
    'Decide whether to make the Data URI Creator page publicly available to all users or privately only to administrators.  For more granular access control, use the !permissions_page.',
    array(
      '!permissions_page' => l(t('Data URI Creator permissions'), 'admin/people/permissions', ($url_options + array('fragment' => 'module-data_uri_creator'))),
    )
  );
  $help .= '</li></ul>';

  $help .= '<h3 id="data-uri-creator-operation">' . t('Operation') . '</h3>';
  $help .= '<p>';
  $help .= t(
    'The Data URI Creator page is used to create a Data URI from an uploaded file, such as from a small PNG image. The utility page is available !here.  From the utility page, select the <em>Data File</em> to be converted to a Data URI. Then, click on the <em>Create Data URI</em> button. Upon success, the Data URI will be displayed in a text box. One can now <em>Select All</em> text from the box, and then <em>Copy</em> and <em>Paste</em> the data where needed. Repeat the process, if desired.',
    array(
      '!here' => l(t('here'), DATA_URI_CREATOR_PAGE_PATH, $url_options),
    )
  );
  $help .= '</p>';

  return $help;
}
