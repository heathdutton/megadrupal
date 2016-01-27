
<?php
/**
 * commerce_order2pdf_settings_form.
 */
function commerce_order2pdf_settings_form($form, &$form_state) {
  $default_header = "
  <table>
  <tr>
    <td>
      [site:name]
    <td>
    <td style=\"text-align: right\">
      <b>Order nr: [commerce-order:order-number]</b><br />
      <b>Created at:</b> [commerce-order:created]<br />
    </td>
  </tr>
  </table>";
  $query_destination =  array('query' => array(drupal_get_destination()));
  $form['order2pdf_info'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . t('Edit Order2pdf !order_view and !line_items views as needed.',
      array('!order_view' => l(t('order view'), 'admin/structure/views/view/commerce_order2pdf/edit', $query_destination), '!line_items' => l(t('line items'), 'admin/structure/views/view/commerce_order2pdf_line_item_table/edit', $query_destination))
    ) .'</p>'
  );
  $form['order2pdf_pdf_header'] = array(
    '#type' => 'textarea',
    '#title' => t('Set PDF order header'),
    '#cols' => 5,
    '#default_value' => variable_get('order2pdf_pdf_header', $default_header),
    '#description' => t('Create header to your commerce order PDF file. Just write some html and use tokens.'),
  );
  $form['order2pdf_pdf_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Set PDF file name'),
    '#default_value' => variable_get('order2pdf_pdf_title', 'Order_[commerce-order:order-id]'),
    '#description' => t('Add your own title for generated pdf file.'),
    '#required' => TRUE,
  );
  $form['order2pdf_pdf_style_file'] = array(
    '#type' => 'textfield',
    '#title' => t('Inline CSS files'),
    '#default_value' => variable_get('order2pdf_pdf_style_file',
      drupal_get_path('module', 'commerce_order2pdf') . '/commerce_order2pdf.css'),
    '#required' => TRUE,
  );
  $form['token_help'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('commerce-order'),
    '#global_types' => TRUE, // A boolean TRUE or FALSE whether to include 'global' context tokens like [current-user:*] or [site:*]. Defaults to TRUE.
    '#click_insert' => TRUE, // A boolean whether to include the 'Click this token to insert in into the the focused textfield' JavaScript functionality. Defaults to TRUE.
    '#dialog' => TRUE,
  );
  return system_settings_form($form);
}
