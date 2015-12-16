
/**
 * Smackdown voting through AJAX
 *
 * Provide a means to vote on our nodereference fields and redirect to a new
 * smackdown node. Javascript techniques accredited to starbow's popups.js.
 */
(function ($) {
  /**
   * Create the smackdown object/namespace.
   */
  Drupal.smackdown = function() {};

  /**
   * Attach the smackdown behavior to nodereferences on the page.
   *
   * @param context
   *   The jQuery object to apply the behaviors to.
   */
  Drupal.behaviors.smackdown = {
    attach: function(context, settings) {
      if (typeof Drupal.settings.smackdown !== 'undefined') {
        var field_1 = 'field field-name-' + Drupal.settings.smackdown.field1.name.replace(/_/g, '-');
        var field_2 = 'field field-name-' + Drupal.settings.smackdown.field2.name.replace(/_/g, '-');
        if (Drupal.settings.smackdownPerm == 1) {
          Drupal.smackdown.attachVote(context, field_1 + ' field-type-node-reference', Drupal.settings.smackdown.field1.nid);
          Drupal.smackdown.attachVote(context, field_2 + ' field-type-node-reference', Drupal.settings.smackdown.field2.nid);
        }
      } else {
        Drupal.smackdown.attachNotice(context, field_1 + ' field-type-node-reference field-label-above');
        Drupal.smackdown.attachNotice(context, field_2 + ' field-type-node-reference field-label-above');
      }
    }
  };

  /**
   * Attach the smackdown behavior to a particular link.
   *
   * @param context
   * @param selector
   * @param nid
   *   jQuery selector for links to attach behavior to.
   */
  Drupal.smackdown.attachVote = function(context, selector, nid) {
    //a regex for selecting the correct selector
    $('div[class^="' + selector + '"]').each(function() {
      var $element = $(this).find('.field-items').children();
      $element.attr('rel', nid).addClass('smackdown-processed');

      // Attach the on-click popup behavior to the element.
      $element.click(function(e) {
        Drupal.theme('voting', $element);
        var nid = $element.attr('rel'); // not compatible with clean urls
        var sid = Drupal.settings.smackdown.sid;
        // post nid and context to smackdown/vote
        $.ajax({
          url: Drupal.settings.basePath + 'smackdown/vote/' + sid + '/' + nid + '/' + Drupal.settings.smackdown.token,
          dataType: 'json',
          success: function(json) {
            location.href = json.location;
          }
        });
        return false;
      });
    });
  };

  Drupal.smackdown.attachNotice = function(context, selector) {
    $(selector, context).each(function() {
      var $element = $(this);
      $element.click(function(e) {
        Drupal.theme('notice', $element);
        return false;
      });
    });
  };

  Drupal.theme.prototype.voting = function(element) {
    var output = "<div id='voting-indicator'>" + Drupal.t('Voting...') + "</div>";
    element.parent().css({'background-color':'#ffc'});
    return element.parent().append(output);
  };

  Drupal.theme.prototype.notice = function(element) {
    var output = "<div id='voting-indicator'>" + Drupal.t('You do not have sufficient rights to vote.') + "</div>";
    element.parent().css({'background-color':'#ffc'});
    return element.parent().append(output);
  };
})(jQuery);
