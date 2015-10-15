/**
 * Implements theme_update_report().
 */
function <?php print $basename; ?>_update_report(\$variables) {
  ${1:\$data = \$variables['data'];

  \$last = variable_get('update_last_check', 0);
  \$output = theme('update_last_check', array('last' => \$last));

  if (!is_array(\$data)) {
    \$output .= '<p>' . \$data . '</p>';
    return \$output;
  \}

  \$header = array();
  \$rows = array();

  \$notification_level = variable_get('update_notification_threshold', 'all');

  // Create an array of status values keyed by module or theme name, since
  // we'll need this while generating the report if we have to cross reference
  // anything (e.g. subthemes which have base themes missing an update).
  foreach (\$data as \$project) {
    foreach (\$project['includes'] as \$key => \$name) {
      \$status[\$key] = \$project['status'];
    \}
  \}

  foreach (\$data as \$project) {
    switch (\$project['status']) {
      case UPDATE_CURRENT:
        \$class = 'ok';
        \$icon = theme('image', array('path' => 'misc/watchdog-ok.png', 'alt' => t('ok'), 'title' => t('ok')));
        break;
      case UPDATE_UNKNOWN:
      case UPDATE_FETCH_PENDING:
      case UPDATE_NOT_FETCHED:
        \$class = 'unknown';
        \$icon = theme('image', array('path' => 'misc/watchdog-warning.png', 'alt' => t('warning'), 'title' => t('warning')));
        break;
      case UPDATE_NOT_SECURE:
      case UPDATE_REVOKED:
      case UPDATE_NOT_SUPPORTED:
        \$class = 'error';
        \$icon = theme('image', array('path' => 'misc/watchdog-error.png', 'alt' => t('error'), 'title' => t('error')));
        break;
      case UPDATE_NOT_CHECKED:
      case UPDATE_NOT_CURRENT:
      default:
        \$class = 'warning';
        \$icon = theme('image', array('path' => 'misc/watchdog-warning.png', 'alt' => t('warning'), 'title' => t('warning')));
        break;
    \}

    \$row = '<div class="version-status">';
    \$status_label = theme('update_status_label', array('status' => \$project['status']));
    \$row .= !empty(\$status_label) ? \$status_label : check_plain(\$project['reason']);
    \$row .= '<span class="icon">' . \$icon . '</span>';
    \$row .= "</div>\n";

    \$row .= '<div class="project">';
    if (isset(\$project['title'])) {
      if (isset(\$project['link'])) {
        \$row .= l(\$project['title'], \$project['link']);
      \}
      else {
        \$row .= check_plain(\$project['title']);
      \}
    \}
    else {
      \$row .= check_plain(\$project['name']);
    \}
    \$row .= ' ' . check_plain(\$project['existing_version']);
    if (\$project['install_type'] == 'dev' && !empty(\$project['datestamp'])) {
      \$row .= ' <span class="version-date">(' . format_date(\$project['datestamp'], 'custom', 'Y-M-d') . ')</span>';
    \}
    \$row .= "</div>\n";

    \$versions_inner = '';
    \$security_class = array();
    \$version_class = array();
    if (isset(\$project['recommended'])) {
      if (\$project['status'] != UPDATE_CURRENT || \$project['existing_version'] !== \$project['recommended']) {

        // First, figure out what to recommend.
        // If there's only 1 security update and it has the same version we're
        // recommending, give it the same CSS class as if it was recommended,
        // but don't print out a separate "Recommended" line for this project.
        if (!empty(\$project['security updates']) && count(\$project['security updates']) == 1 && \$project['security updates'][0]['version'] === \$project['recommended']) {
          \$security_class[] = 'version-recommended';
          \$security_class[] = 'version-recommended-strong';
        \}
        else {
          \$version_class[] = 'version-recommended';
          // Apply an extra class if we're displaying both a recommended
          // version and anything else for an extra visual hint.
          if (\$project['recommended'] !== \$project['latest_version']
              || !empty(\$project['also'])
              || (\$project['install_type'] == 'dev'
                && isset(\$project['dev_version'])
                && \$project['latest_version'] !== \$project['dev_version']
                && \$project['recommended'] !== \$project['dev_version'])
              || (isset(\$project['security updates'][0])
                && \$project['recommended'] !== \$project['security updates'][0])
              ) {
            \$version_class[] = 'version-recommended-strong';
          \}
          \$versions_inner .= theme('update_version', array('version' => \$project['releases'][\$project['recommended']], 'tag' => t('Recommended version:'), 'class' => \$version_class));
        \}

        // Now, print any security updates.
        if (!empty(\$project['security updates'])) {
          \$security_class[] = 'version-security';
          foreach (\$project['security updates'] as \$security_update) {
            \$versions_inner .= theme('update_version', array('version' => \$security_update, 'tag' => t('Security update:'), 'class' => \$security_class));
          \}
        \}
      \}

      if (\$project['recommended'] !== \$project['latest_version']) {
        \$versions_inner .= theme('update_version', array('version' => \$project['releases'][\$project['latest_version']], 'tag' => t('Latest version:'), 'class' => array('version-latest')));
      \}
      if (\$project['install_type'] == 'dev'
          && \$project['status'] != UPDATE_CURRENT
          && isset(\$project['dev_version'])
          && \$project['recommended'] !== \$project['dev_version']) {
        \$versions_inner .= theme('update_version', array('version' => \$project['releases'][\$project['dev_version']], 'tag' => t('Development version:'), 'class' => array('version-latest')));
      \}
    \}

    if (isset(\$project['also'])) {
      foreach (\$project['also'] as \$also) {
        \$versions_inner .= theme('update_version', array('version' => \$project['releases'][\$also], 'tag' => t('Also available:'), 'class' => array('version-also-available')));
      \}
    \}

    if (!empty(\$versions_inner)) {
      \$row .= "<div class=\"versions\">\n" . \$versions_inner . "</div>\n";
    \}
    \$row .= "<div class=\"info\">\n";
    if (!empty(\$project['extra'])) {
      \$row .= '<div class="extra">' . "\n";
      foreach (\$project['extra'] as \$key => \$value) {
        \$row .= '<div class="' . implode(' ', \$value['class']) . '">';
        \$row .= check_plain(\$value['label']) . ': ';
        \$row .= drupal_placeholder(\$value['data']);
        \$row .= "</div>\n";
      \}
      \$row .= "</div>\n";  // extra div.
    \}

    \$row .= '<div class="includes">';
    sort(\$project['includes']);
    if (!empty(\$project['disabled'])) {
      sort(\$project['disabled']);
      // Make sure we start with a clean slate for each project in the report.
      \$includes_items = array();
      \$row .= t('Includes:');
      \$includes_items[] = t('Enabled: %includes', array('%includes' => implode(', ', \$project['includes'])));
      \$includes_items[] = t('Disabled: %disabled', array('%disabled' => implode(', ', \$project['disabled'])));
      \$row .= theme('item_list', array('items' => \$includes_items));
    \}
    else {
      \$row .= t('Includes: %includes', array('%includes' => implode(', ', \$project['includes'])));
    \}
    \$row .= "</div>\n";

    if (!empty(\$project['base_themes'])) {
      \$row .= '<div class="basethemes">';
      asort(\$project['base_themes']);
      \$base_themes = array();
      foreach (\$project['base_themes'] as \$base_key => \$base_theme) {
        switch (\$status[\$base_key]) {
          case UPDATE_NOT_SECURE:
          case UPDATE_REVOKED:
          case UPDATE_NOT_SUPPORTED:
            \$base_themes[] = t('%base_theme (!base_label)', array('%base_theme' => \$base_theme, '!base_label' => theme('update_status_label', array('status' => \$status[\$base_key]))));
            break;

          default:
            \$base_themes[] = drupal_placeholder(\$base_theme);
        \}
      \}
      \$row .= t('Depends on: !basethemes', array('!basethemes' => implode(', ', \$base_themes)));
      \$row .= "</div>\n";
    \}

    if (!empty(\$project['sub_themes'])) {
      \$row .= '<div class="subthemes">';
      sort(\$project['sub_themes']);
      \$row .= t('Required by: %subthemes', array('%subthemes' => implode(', ', \$project['sub_themes'])));
      \$row .= "</div>\n";
    \}

    \$row .= "</div>\n"; // info div.

    if (!isset(\$rows[\$project['project_type']])) {
      \$rows[\$project['project_type']] = array();
    \}
    \$row_key = isset(\$project['title']) ? drupal_strtolower(\$project['title']) : drupal_strtolower(\$project['name']);
    \$rows[\$project['project_type']][\$row_key] = array(
      'class' => array(\$class),
      'data' => array(\$row),
    );
  \}

  \$project_types = array(
    'core' => t('Drupal core'),
    'module' => t('Modules'),
    'theme' => t('Themes'),
    'module-disabled' => t('Disabled modules'),
    'theme-disabled' => t('Disabled themes'),
  );
  foreach (\$project_types as \$type_name => \$type_label) {
    if (!empty(\$rows[\$type_name])) {
      ksort(\$rows[\$type_name]);
      \$output .= "\n<h3>" . \$type_label . "</h3>\n";
      \$output .= theme('table', array('header' => \$header, 'rows' => \$rows[\$type_name], 'attributes' => array('class' => array('update'))));
    \}
  \}
  drupal_add_css(drupal_get_path('module', 'update') . '/update.css');
  return \$output;}
}

$2