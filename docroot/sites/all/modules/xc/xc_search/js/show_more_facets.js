/**
 * @file
 * JavaScript functions for XC Search module
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
(function ($) {
  $(document).ready(function() {
    var chart = new JSChart('jschart_' + Drupal.settings.xc_search.facet_name, 'bar');
    chart.setDataArray(Drupal.settings.xc_search.facet_array.slice(0, 100));
    chart.setTitle('Top ' + Drupal.settings.xc_search.facet_count + ' ' + Drupal.settings.xc_search.facet_label);
    chart.setSize(900,300);
    chart.setShowXValues(false);
    chart.setAxisNameX(Drupal.t('Terms'));
    chart.setAxisNameY(Drupal.t('Documents'));
    chart.setAxisPaddingLeft(70);
    chart.draw();
  });
}(jQuery));