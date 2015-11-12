/**
 * @file highcharttable.js
 * 
 * Takes parameters set on the HighchartTable configuration page and invokes
 * HighchartTable chart-from-table function.
 */
(function ($) {

  Drupal.behaviors.highcharttable = {

    attach: function(context, settings) {

      $(settings.highcharttable, context).each(function() {

        // settings.highcharttable is an array of params indexed by selectors as
        // entered on the HighchartTable config page. 1 selector per decoration.
        // In addition global config params are kept under 'global'.
        var linkStyle = null;
        $.each(this, function(selector, params) {

          if (selector === 'global') {
            linkStyle = params['contextual-links'];
            if (linkStyle !== 'off') {
              var linkHtml = Drupal.behaviors.highcharttable.insertContextLinkHtml(linkStyle);
              $(linkHtml).insertBefore('table');
            }
          }
          else {
            // table selector
            var decoratedTables = $(selector);
            if (decoratedTables.length > 0) {
              // Set all params on the special "data-graph-..." attributes.
              for (var dataKey in params) {
                if (dataKey !== 'hide-table' && dataKey.substring(0, 10) !== 'highchart-' && params[dataKey] !== null) {
                  decoratedTables.attr('data-graph-' + dataKey, params[dataKey]);
                }
              }
              // Remove insert link before adding other links and the chart.
              decoratedTables.prev('.highcharttable-region').remove();

              if (linkStyle !== 'off') {
                var linkHtml = Drupal.behaviors.highcharttable.configureDeleteContextLinksHtml(linkStyle);
                $(linkHtml).insertBefore(decoratedTables);
              }

              // Set options for the highchart object.
              decoratedTables.bind('highchartTable.beforeRender', function(event, highChartConfig) {
                selector = decoratedTables.selector;
                var params = settings.highcharttable[selector];
                for (var dataKey in params) {
                  if (dataKey.substring(0, 10) === 'highchart-' && params[dataKey] !== null) {
                    var _dataKey = dataKey.slice(10, dataKey.length);
                    highChartConfig[_dataKey] = params[dataKey];
                  }
                }
              });

              decoratedTables.highchartTable();

              if (params['hide-table']) {
                decoratedTables.hide();
              }
            }
          }
        });
      });
    },

    insertContextLinkHtml: function(linkStyle) {
      var basePath = Drupal.settings.basePath;
      var thisPage = window.location.pathname.substr(basePath.length);
      var linkText = Drupal.t('Insert chart from table');
      // Note: this link assumes Clean URLs is enabled.
      var link = '<li><a class="highcharttable-insert" href="' + basePath + 'highcharttable/contextual-link/insert?destination=' + thisPage + '">' + linkText + '</a></li>';

      if (linkStyle === 'plain') {
        return '<div class="highcharttable-region">' +
            '<ul>' + link + '</ul>' +
          '</div>';
      }
      return '<div class="highcharttable-region contextual-links-region">' +
          '<div class="contextual-links-wrapper">' +
            '<ul class="contextual-links">' + link + '</ul>' +
          '</div>' +
        '</div>';
    },

    configureDeleteContextLinksHtml: function(linkStyle) {
      var basePath = Drupal.settings.basePath;
      var thisPage = window.location.pathname.substr(basePath.length);
      var link1Text = Drupal.t('Configure this chart');
      var link2Text = Drupal.t('Delete chart(s) from page');
      var links =
        '<li><a class="highcharttable-configure" href="' + basePath + 'admin/config/content/highcharttable?destination=' + thisPage + '">' + link1Text + '</a></li>' +
        '<li><a class="highcharttable-delete"  href="' + basePath + 'highcharttable/contextual-link/delete?destination=' + thisPage + '">' + link2Text + '</a></li>';

      if (linkStyle === 'plain') {
        return '<div class="highcharttable-region">' +
              '<ul>' + links + '</ul>' +
            '</div>';
      }
      return '<div class="highcharttable-region contextual-links-region">' +
          '<div class="contextual-links-wrapper">' +
            '<ul class="contextual-links">' + links + '</ul>' +
          '</div>' +
        '</div>';
    }
  };

}) (jQuery);

/**
 * Highcharts.numberformat(value, decimals, decPoint, thousandsSep)
 * decPoint (e.g., '.') and thousandsSep (',') are taken from lang/locale.
 */

highcharttableDollarFormatter = function (value) {
  return '$ ' + Highcharts.numberFormat(value, -1);
}

highcharttableEuroFormatter = function (value) {
  return '€ ' + Highcharts.numberFormat(value, -1);
}

highcharttablePoundFormatter = function (value) {
  return '£ ' + Highcharts.numberFormat(value, -1);
}

highcharttableNumberFormatter1Decimal = function (value) {
  return Highcharts.numberFormat(value, 1);
}

highcharttableNumberFormatter2Decimals = function (value) {
  return Highcharts.numberFormat(value, 2);
}

highcharttableNumberFormatter3Decimals = function (value) {
  return Highcharts.numberFormat(value, 3);
}

highcharttableDollarFormatter2Decimals = function (value) {
  return '$ ' + Highcharts.numberFormat(value, 2);
}

highcharttableEuroFormatter2Decimals = function (value) {
  return '€ ' + Highcharts.numberFormat(value, 2);
}

highcharttablePoundFormatter2Decimals = function (value) {
  return '£ ' + Highcharts.numberFormat(value, 2);
}

highcharttablePercentFormatter = function (value) {
  return value + ' %';
}
