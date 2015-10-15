/**
 * Implements theme_taxonomy_overview_vocabularies().
 */
function <?php print $basename; ?>_taxonomy_overview_vocabularies(\$variables) {
  ${1:\$form = \$variables['form'];

  \$rows = array();

  foreach (element_children(\$form) as \$key) {
    if (isset(\$form[\$key]['name'])) {
      \$vocabulary = &\$form[\$key];

      \$row = array();
      \$row[] = drupal_render(\$vocabulary['name']);
      if (isset(\$vocabulary['weight'])) {
        \$vocabulary['weight']['#attributes']['class'] = array('vocabulary-weight');
        \$row[] = drupal_render(\$vocabulary['weight']);
      \}
      \$row[] = drupal_render(\$vocabulary['edit']);
      \$row[] = drupal_render(\$vocabulary['list']);
      \$row[] = drupal_render(\$vocabulary['add']);
      \$rows[] = array('data' => \$row, 'class' => array('draggable'));
    \}
  \}

  \$header = array(t('Vocabulary name'));
  if (isset(\$form['actions'])) {
    \$header[] = t('Weight');
    drupal_add_tabledrag('taxonomy', 'order', 'sibling', 'vocabulary-weight');
  \}
  \$header[] = array('data' => t('Operations'), 'colspan' => '3');
  return theme('table', array('header' => \$header, 'rows' => \$rows, 'empty' => t('No vocabularies available. <a href="@link">Add vocabulary</a>.', array('@link' => url('admin/structure/taxonomy/add'))), 'attributes' => array('id' => 'taxonomy'))) . drupal_render_children(\$form);}
}

$2