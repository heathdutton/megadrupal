<?php
/**
 * @file
 * The Affiliate module's theme implementation to display an affiliate link.
 *
 * Available variables:
 * - $url: the link to which affiliates may point their users in order for the affiliate
 *   to receive credit.
 * - $display: the markup to be displayed inside the $url, like this:
 *    <a href="URL">DISPLAY</a>. 
 * - $node: the fully-loaded node object, if the current page being viewed is a node.
 *
 * Example:
 * If you run a gardening video tube site whose niche is focused on attractive, young,
 * ripe plants during harvest season, then you may want your affiliates' links to display
 * more than just the plain text title inside said link. By default, the embed code that
 * the affiliates will see for each node will contain the node title and will look
 * something like this:
 *
 * <a href="http://www.nicemelons.com/affiliate/6/node|9">Best watermelons I've ever seen</a>
 *
 * While that's all well and good, you might be well-advised by a martketer to use visual
 * depictions or previews of what the potential viewer will see if they click your link,
 * as opposed to just a text description.
 *
 * If such is the case, assuming the link in question is for a node (as opposed to, say,
 * the "contact us" page), and that you've added an image field for that node type whose
 * machine name is "field_image", and that you want the image from that field displayed in
 * place of the plain text title, you could copy this file to your theme's directory and
 * then change your newly-copied file so that the top of it looks similar to this:
 *
 *  if (isset($node) && isset($node->field_image['und'][0]['uri'])) {
 *    $inner = '<img src="' . file_create_url($node->field_image['und'][0]['uri']) . '" />';
 *  }
 *  else if (isset($display)) {
 *    $inner = $display;
 *  }
 *  else if (isset($node) && !empty($node->title)) {
 *    $inner = $node->title;
 *  }
 *  else {
 *    $inner = drupal_get_title();
 *  }
 *  $inner = check_plain($inner);
 *  
 * leaving the rest of the file as is. This would result in the affiliate link now looking
 * something like this:
 *
 * <a href="http://www.nicemelons.com/affiliate/6/node|9"><img src="http://www.nicemelons.com/sites/default/files/melonpics/bestmelons.jpg" /></a>
 *
 * @see affiliate_fetch_url()
 */
?>
<?php
  if (isset($display)) {
    $inner = $display;
  }
  else if (isset($node) && !empty($node->title)) {
    $inner = $node->title;
  }
  else {
    $inner = drupal_get_title();
  }
  $inner = check_plain($inner);
?>
  <div class="clearfix"></div>
  <div id="affiliate-click-link"><a href="#null"><?php print t('Display affiliate link'); ?></a></div>
  <div style="display: none;" id="affiliate-url" class="affiliate-url-node-markup">
    <div class="affiliate-url-prefix"><strong><?php print filter_xss(variable_get('affiliate_module_affiliate_link_text', t('Copy the code below and paste it into your website'))); ?></strong></div>
    <textarea style="width: 100%;" rows="2" readonly="readonly" onclick="this.select()">&lt;a href="<?php print $url; ?>"&gt;<?php print $inner; ?>&lt;/a&gt;</textarea>
  </div>
