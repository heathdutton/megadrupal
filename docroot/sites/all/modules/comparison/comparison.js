/**
 * @file
 * Javascript flag comparison.
 */

(function ($) {

  Drupal.behaviors.flag_comparison = {
    attach: function (context, settings) {
      $.get(Drupal.settings.basePath + 'comparison/message', null, message);
      $('li.node-comparison').each(function(index, value) {
        $(value).append('<span class="comparison-throbber">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp&nbsp;</span>');
      });
      $('li.node-comparison').find('a.comparison-node').click(checkFlag);
    }
  };

  var message = function(response) {
    if (response.status) {
      $('li.node-comparison').each( function(i,val) {
        var product = $(val).find('a.comparison-node').attr('class')
          .split(' ')[1].split('-')[2];
        if (response.nodes[product]) {
          // Check the box.
          var src = $('a.comparison-node-' + product).find('img').attr('src');
          src = src.replace('compare.gif', 'compare_on.gif');
          $('a.comparison-node-' + product).find('img').attr('src', src);
          addMessage(response.count, product);
        }
        else {
          // Uncheck the box.
          var src = $('a.comparison-node-' + product).find('img').attr('src');
          src = src.replace('compare_on.gif', 'compare.gif');
          $('a.comparison-node-' + product).find('img').attr('src', src);
          $('a.comparison-node-' + product).parent()
            .find('span.comparison-message').remove();
        }
        $(val).find('span.comparison-throbber').css('display', 'none');
      });
    }
  };

  var checkFlag = function(event) {
    $(event.target).parent().parent().find('.comparison-throbber')
      .css('display', 'inline');
    var classes = $(event.target).parent().attr('class').split(' ');
    var nodeClass = $(event.target).parent().attr('class').split(' ')[1];
    var node = nodeClass.split('-')[2];
    var src = $(event.target).attr('src');
    if (src.indexOf('compare.gif') != -1) {
      $.get(Drupal.settings.basePath + 'comparison/' + node + '/flag',
        null, updateMessage);
      return false;
    }
    if (src.indexOf('compare_on.gif') != -1) {
      $.get(Drupal.settings.basePath + 'comparison/' + node + '/unflag',
        null, updateMessage);
      return false;
    }
    return false;
  };

  var updateMessage = function(response) {
    $.get(Drupal.settings.basePath + 'comparison/message', null, message);
  };

  var addMessage = function(count, nid) {
    $('a.comparison-node-' + nid).parent().find('span.comparison-message')
      .remove();
    var newHTML = '<span class="comparison-message">';
    if (count == 1) {
      newHTML += 'Select another item to compare';
    }
    else {
      var button = '<button type="button">Compare Now!</button>';
      newHTML += '<a href="' + Drupal.settings.basePath +
        'product-comparison">' + button + '</a>';
    }
    newHTML += '</span>';
    $('a.comparison-node-' + nid).parent().append(newHTML);
  };
})(jQuery);
