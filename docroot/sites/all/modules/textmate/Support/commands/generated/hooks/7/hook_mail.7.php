/**
 * Implements hook_mail().
 */
function <?php print $basename; ?>_mail(\$key, &\$message, \$params) {
  ${1:\$account = \$params['account'];
  \$context = \$params['context'];
  \$variables = array(
    '%site_name' => variable_get('site_name', 'Drupal'),
    '%username' => format_username(\$account),
  );
  if (\$context['hook'] == 'taxonomy') {
    \$entity = \$params['entity'];
    \$vocabulary = taxonomy_vocabulary_load(\$entity->vid);
    \$variables += array(
      '%term_name' => \$entity->name,
      '%term_description' => \$entity->description,
      '%term_id' => \$entity->tid,
      '%vocabulary_name' => \$vocabulary->name,
      '%vocabulary_description' => \$vocabulary->description,
      '%vocabulary_id' => \$vocabulary->vid,
    );
  \}

  // Node-based variable translation is only available if we have a node.
  if (isset(\$params['node'])) {
    \$node = \$params['node'];
    \$variables += array(
      '%uid' => \$node->uid,
      '%node_url' => url('node/' . \$node->nid, array('absolute' => TRUE)),
      '%node_type' => node_type_get_name(\$node),
      '%title' => \$node->title,
      '%teaser' => \$node->teaser,
      '%body' => \$node->body,
    );
  \}
  \$subject = strtr(\$context['subject'], \$variables);
  \$body = strtr(\$context['message'], \$variables);
  \$message['subject'] .= str_replace(array("\r", "\n"), '', \$subject);
  \$message['body'][] = drupal_html_to_text(\$body);}
}

$2