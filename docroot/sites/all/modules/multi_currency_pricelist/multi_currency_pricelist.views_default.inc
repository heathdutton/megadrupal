<?php
/**
 * @file
 * multi_currency_pricelist.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function multi_currency_pricelist_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'pricelist';
  $view->description = 'Pricelist with a tab for each accepted currency.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Pricelist';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Pricelist';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'field_pricelist_section',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'field_pricelist_section' => 'field_pricelist_section',
    'title' => 'title',
    'edit_node' => 'edit_node',
    'field_price' => 'field_price',
    'php' => 'php',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_pricelist_section' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'field_price' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-right',
      'separator' => '',
      'empty_column' => 0,
    ),
    'php' => array(
      'align' => 'views-align-right',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<b><?php  $from = \'EUR\'; // Base currency code.
   $to   = \'EGP\'; // Converted currency code.
   $amt  = \'1\';
  if (module_exists(\'currency\')) {
    $exchange_rate = CurrencyExchanger::load($from, $to, $amt);
    $ret = number_format(round(bcmul($exchange_rate, $amt, 3), 2), 2);
    $xch = \'1 \'. $from . \' (base currency) = \' . $ret . \' \' . $to;
  }else{
    $xch = \'error\';
  };
print $xch;
?>
 on <?php print date("d-m-Y", strtotime(\'now\')); ?> according <a href="http://finance.yahoo.com/currency-converter/#from=<?php print $from; ?>;to=<?php print $to; ?>;amt=1" title="Currency converter">Yahoo!Finance</a>.  <br />Accepted currencies are EUR, EGP, GBP and USD. Conversion results might be rounded. <br />Credit card payments will be in the local currency (<a href="/pricelist/egp" >Egyptian Pound</a>). <br />At the moment of order/payment the latest <a href="/pricelist" >online pricelist</a> is used. Printed versions are approximate.</b><br />';
  $handler->display->display_options['header']['area']['format'] = 'php_code';
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['empty'] = TRUE;
  $handler->display->display_options['footer']['area']['content'] = '<?php
global $user;
if (($user->uid) == 1): ?><br />
<a href="#overlay=admin/structure/taxonomy/pricelist_section">Order/edit pricelist sections</a> | <a href="#overlay=admin/structure/nodequeue">Order items</a> | <a href="#overlay=node/add/pricelist-item">Add item</a>
<?php endif;
?>';
  $handler->display->display_options['footer']['area']['format'] = 'php_code';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Explanation why the view could be empty';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = '<p>&nbsp;</p>
*** NO RESULTS ***
<p>&nbsp;</p>
Check if:
<ul>
  <li><a href="/admin/structure/nodequeue">nodequeues</a> exist and if they contain any pricelist items</li>
  <li>any terms exist in the taxonomy vocabulary <a href="/admin/structure/taxonomy/pricelist_section">Pricelist sections</a></li>
  <li>any <a href="/admin/content">pricelist items</a> exist and if they are assigned to a queue (see tab \'Nodequeue\')</li>
  <li>and those have a valid taxonomy term referenced.</li>
</ul>
  ';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  /* Relationship: Content: Pricelist section (field_pricelist_section) */
  $handler->display->display_options['relationships']['field_pricelist_section_tid']['id'] = 'field_pricelist_section_tid';
  $handler->display->display_options['relationships']['field_pricelist_section_tid']['table'] = 'field_data_field_pricelist_section';
  $handler->display->display_options['relationships']['field_pricelist_section_tid']['field'] = 'field_pricelist_section_tid';
  /* Relationship: Nodequeue: Queue */
  $handler->display->display_options['relationships']['nodequeue_rel']['id'] = 'nodequeue_rel';
  $handler->display->display_options['relationships']['nodequeue_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['nodequeue_rel']['field'] = 'nodequeue_rel';
  $handler->display->display_options['relationships']['nodequeue_rel']['required'] = TRUE;
  $handler->display->display_options['relationships']['nodequeue_rel']['limit'] = 1;
  $handler->display->display_options['relationships']['nodequeue_rel']['names'] = array(
    'pricelist_order_items' => 'pricelist_order_items',
    'pricelist_items_order' => 0,
  );
  /* Field: Content: Pricelist section */
  $handler->display->display_options['fields']['field_pricelist_section']['id'] = 'field_pricelist_section';
  $handler->display->display_options['fields']['field_pricelist_section']['table'] = 'field_data_field_pricelist_section';
  $handler->display->display_options['fields']['field_pricelist_section']['field'] = 'field_pricelist_section';
  $handler->display->display_options['fields']['field_pricelist_section']['label'] = '';
  $handler->display->display_options['fields']['field_pricelist_section']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_pricelist_section']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_pricelist_section']['alter']['text'] = '<h2>[field_pricelist_section]</h2>';
  $handler->display->display_options['fields']['field_pricelist_section']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_pricelist_section']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
  /* Field: Content: Price */
  $handler->display->display_options['fields']['field_price']['id'] = 'field_price';
  $handler->display->display_options['fields']['field_price']['table'] = 'field_data_field_price';
  $handler->display->display_options['fields']['field_price']['field'] = 'field_price';
  $handler->display->display_options['fields']['field_price']['click_sort_column'] = 'amount';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'Price';
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_output'] = '<?php  $from = $data->field_field_price[0][\'raw\'][\'currency\'];
   $to   = \'EGP\';
   $amt  = $data->field_field_price[0][\'raw\'][\'amount\'];
  if (module_exists(\'currency\') && $amt!=0) {
    $exchange_rate = CurrencyExchanger::load($from, $to, $amt);
    $ret = bcmul($exchange_rate, $amt);
    $node_field[0][\'value\'] = $ret . \' EGP\';
  }else{
    $node_field[0][\'value\'] = \'on request\';
  };
print $node_field[0][\'value\'];
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Sort criterion: Taxonomy term: Weight */
  $handler->display->display_options['sorts']['weight']['id'] = 'weight';
  $handler->display->display_options['sorts']['weight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['sorts']['weight']['field'] = 'weight';
  $handler->display->display_options['sorts']['weight']['relationship'] = 'field_pricelist_section_tid';
  /* Sort criterion: Nodequeue: Position */
  $handler->display->display_options['sorts']['position']['id'] = 'position';
  $handler->display->display_options['sorts']['position']['table'] = 'nodequeue_nodes';
  $handler->display->display_options['sorts']['position']['field'] = 'position';
  $handler->display->display_options['sorts']['position']['relationship'] = 'nodequeue_rel';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'pricelist_item' => 'pricelist_item',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';

  /* Display: EUR */
  $handler = $view->new_display('page', 'EUR', 'page');
  $handler->display->display_options['display_description'] = 'Pricelist in Euro';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Pricelist section */
  $handler->display->display_options['fields']['field_pricelist_section']['id'] = 'field_pricelist_section';
  $handler->display->display_options['fields']['field_pricelist_section']['table'] = 'field_data_field_pricelist_section';
  $handler->display->display_options['fields']['field_pricelist_section']['field'] = 'field_pricelist_section';
  $handler->display->display_options['fields']['field_pricelist_section']['label'] = '';
  $handler->display->display_options['fields']['field_pricelist_section']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_pricelist_section']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_pricelist_section']['alter']['text'] = '<h2>[field_pricelist_section]</h2>';
  $handler->display->display_options['fields']['field_pricelist_section']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_pricelist_section']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'Price';
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_output'] = '<?php  $from = $data->field_field_price[0][\'raw\'][\'currency\'];
   $to   = \'EUR\';
   $amt  = $data->field_field_price[0][\'raw\'][\'amount\'];
  if (module_exists(\'currency\') && $amt!=0) {
    $exchange_rate = CurrencyExchanger::load($from, $to, $amt);
    $ret = number_format(round(bcmul($exchange_rate, $amt, 2), 1), 2);
    $node_field[0][\'value\'] = $ret . \' EUR\';
  }else{
    $node_field[0][\'value\'] = \'on request\';
  };
print $node_field[0][\'value\'];
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Content: Price */
  $handler->display->display_options['fields']['field_price']['id'] = 'field_price';
  $handler->display->display_options['fields']['field_price']['table'] = 'field_data_field_price';
  $handler->display->display_options['fields']['field_price']['field'] = 'field_price';
  $handler->display->display_options['fields']['field_price']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['path'] = 'pricelist/eur';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'EUR';
  $handler->display->display_options['menu']['description'] = 'Euro';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Prices';
  $handler->display->display_options['tab_options']['description'] = 'In all accepted currencies.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'main-menu';

  /* Display: EGP */
  $handler = $view->new_display('page', 'EGP', 'page_1');
  $handler->display->display_options['display_description'] = 'Egyptian Pound';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Pricelist section */
  $handler->display->display_options['fields']['field_pricelist_section']['id'] = 'field_pricelist_section';
  $handler->display->display_options['fields']['field_pricelist_section']['table'] = 'field_data_field_pricelist_section';
  $handler->display->display_options['fields']['field_pricelist_section']['field'] = 'field_pricelist_section';
  $handler->display->display_options['fields']['field_pricelist_section']['label'] = '';
  $handler->display->display_options['fields']['field_pricelist_section']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_pricelist_section']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_pricelist_section']['alter']['text'] = '<h2>[field_pricelist_section]</h2>';
  $handler->display->display_options['fields']['field_pricelist_section']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_pricelist_section']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'Price';
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_output'] = '<?php  $from = $data->field_field_price[0][\'raw\'][\'currency\'];
   $to   = \'EGP\';
   $amt  = $data->field_field_price[0][\'raw\'][\'amount\'];
  if (module_exists(\'currency\') && $amt!=0) {
    $exchange_rate = CurrencyExchanger::load($from, $to, $amt);
    $ret = bcmul($exchange_rate, $amt);
    $node_field[0][\'value\'] = $ret . \' EGP\';
  }else{
    $node_field[0][\'value\'] = \'on request\';
  };
print $node_field[0][\'value\'];
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Content: Price */
  $handler->display->display_options['fields']['field_price']['id'] = 'field_price';
  $handler->display->display_options['fields']['field_price']['table'] = 'field_data_field_price';
  $handler->display->display_options['fields']['field_price']['field'] = 'field_price';
  $handler->display->display_options['fields']['field_price']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['path'] = 'pricelist/egp';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'EGP';
  $handler->display->display_options['menu']['description'] = 'Egyptian Pound';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: GBP */
  $handler = $view->new_display('page', 'GBP', 'page_2');
  $handler->display->display_options['display_description'] = 'British Pound';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<b><?php  $from = \'EUR\'; // Base currency code.
   $to   = \'GBP\'; // Converted currency code.
   $amt  = \'1\';
  if (module_exists(\'currency\')) {
    $exchange_rate = CurrencyExchanger::load($from, $to, $amt);
    $ret = number_format(round(bcmul($exchange_rate, $amt, 3), 2), 2);
    $xch = \'1 \'. $from . \' (base currency) = \' . $ret . \' \' . $to;
  }else{
    $xch = \'error\';
  };
print $xch;
?>
 on <?php print date("d-m-Y", strtotime(\'now\')); ?> according <a href="http://finance.yahoo.com/currency-converter/#from=<?php print $from; ?>;to=<?php print $to; ?>;amt=1" title="Currency converter">Yahoo!Finance</a>.  <br />Accepted currencies are EUR, EGP, GBP and USD. Conversion results might be rounded. <br />Credit card payments will be in the local currency (<a href="/pricelist/egp" >Egyptian Pound</a>). <br />At the moment of order/payment the latest <a href="/pricelist" >online pricelist</a> is used. Printed versions are approximate.</b><br />';
  $handler->display->display_options['header']['area']['format'] = 'php_code';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Pricelist section */
  $handler->display->display_options['fields']['field_pricelist_section']['id'] = 'field_pricelist_section';
  $handler->display->display_options['fields']['field_pricelist_section']['table'] = 'field_data_field_pricelist_section';
  $handler->display->display_options['fields']['field_pricelist_section']['field'] = 'field_pricelist_section';
  $handler->display->display_options['fields']['field_pricelist_section']['label'] = '';
  $handler->display->display_options['fields']['field_pricelist_section']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_pricelist_section']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_pricelist_section']['alter']['text'] = '<h2>[field_pricelist_section]</h2>';
  $handler->display->display_options['fields']['field_pricelist_section']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_pricelist_section']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'Price';
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_output'] = '<?php  $from = $data->field_field_price[0][\'raw\'][\'currency\'];
   $to   = \'GBP\';
   $amt  = $data->field_field_price[0][\'raw\'][\'amount\'];
  if (module_exists(\'currency\') && $amt!=0) {
    $exchange_rate = CurrencyExchanger::load($from, $to, $amt);
    $ret = number_format(round(bcmul($exchange_rate, $amt, 2), 1), 2);
    $node_field[0][\'value\'] = $ret . \' GBP\';
  }else{
    $node_field[0][\'value\'] = \'on request\';
  };
print $node_field[0][\'value\'];
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Content: Price */
  $handler->display->display_options['fields']['field_price']['id'] = 'field_price';
  $handler->display->display_options['fields']['field_price']['table'] = 'field_data_field_price';
  $handler->display->display_options['fields']['field_price']['field'] = 'field_price';
  $handler->display->display_options['fields']['field_price']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['path'] = 'pricelist/gbp';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'GBP';
  $handler->display->display_options['menu']['description'] = 'British Pound Sterling';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: USD */
  $handler = $view->new_display('page', 'USD', 'page_3');
  $handler->display->display_options['display_description'] = 'US Dollar';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<b><?php  $from = \'EUR\'; // Base currency code.
   $to   = \'USD\'; // Converted currency code.
   $amt  = \'1\';
  if (module_exists(\'currency\')) {
    $exchange_rate = CurrencyExchanger::load($from, $to, $amt);
    $ret = number_format(round(bcmul($exchange_rate, $amt, 3), 2), 2);
    $xch = \'1 \'. $from . \' (base currency) = \' . $ret . \' \' . $to;
  }else{
    $xch = \'error\';
  };
print $xch;
?>
 on <?php print date("d-m-Y", strtotime(\'now\')); ?> according <a href="http://finance.yahoo.com/currency-converter/#from=<?php print $from; ?>;to=<?php print $to; ?>;amt=1" title="Currency converter">Yahoo!Finance</a>.  <br />Accepted currencies are EUR, EGP, GBP and USD. Conversion results might be rounded. <br />Credit card payments will be in the local currency (<a href="/pricelist/egp" >Egyptian Pound</a>). <br />At the moment of order/payment the latest <a href="/pricelist" >online pricelist</a> is used. Printed versions are approximate.</b><br />';
  $handler->display->display_options['header']['area']['format'] = 'php_code';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Pricelist section */
  $handler->display->display_options['fields']['field_pricelist_section']['id'] = 'field_pricelist_section';
  $handler->display->display_options['fields']['field_pricelist_section']['table'] = 'field_data_field_pricelist_section';
  $handler->display->display_options['fields']['field_pricelist_section']['field'] = 'field_pricelist_section';
  $handler->display->display_options['fields']['field_pricelist_section']['label'] = '';
  $handler->display->display_options['fields']['field_pricelist_section']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_pricelist_section']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_pricelist_section']['alter']['text'] = '<h2>[field_pricelist_section]</h2>';
  $handler->display->display_options['fields']['field_pricelist_section']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_pricelist_section']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'Price';
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_output'] = '<?php  $from = $data->field_field_price[0][\'raw\'][\'currency\'];
   $to   = \'USD\';
   $amt  = $data->field_field_price[0][\'raw\'][\'amount\'];
  if (module_exists(\'currency\') && $amt!=0) {
    $exchange_rate = CurrencyExchanger::load($from, $to, $amt);
    $ret = number_format(round(bcmul($exchange_rate, $amt, 2), 1), 2);
    $node_field[0][\'value\'] = $ret . \' USD\';
  }else{
    $node_field[0][\'value\'] = \'on request\';
  };
print $node_field[0][\'value\'];
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Content: Price */
  $handler->display->display_options['fields']['field_price']['id'] = 'field_price';
  $handler->display->display_options['fields']['field_price']['table'] = 'field_data_field_price';
  $handler->display->display_options['fields']['field_price']['field'] = 'field_price';
  $handler->display->display_options['fields']['field_price']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['path'] = 'pricelist/usd';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'USD';
  $handler->display->display_options['menu']['description'] = 'US Dollar';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
  $export['pricelist'] = $view;

  return $export;
}
