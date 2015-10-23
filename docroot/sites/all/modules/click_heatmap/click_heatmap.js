
/**
 * If inside the ClickHeat iframe then remove admin menu.
 */
Drupal.behaviors.click_heatmap = function() {
  if (window.location.href != parent.location.href) {
    $('#admin-menu').remove();
  }
}
