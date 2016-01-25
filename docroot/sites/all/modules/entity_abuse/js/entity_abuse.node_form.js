(function ($) {

  Drupal.behaviors.entityAbuseFieldsetSummaries = {
    attach: function (context) {

      // Provide the summary for the node type form.
      $('fieldset.entity-abuse-node-type-settings', context).drupalSetSummary(function(context) {
        var vals = [];

        if ($('input[name="entity_abuse_node_enabled"]:checked', context).length) {
          vals.push(Drupal.t('Abuses for nodes: enabled', {}, {}));
        }
        else {
          vals.push(Drupal.t('Abuses for nodes: disabled', {}, {}));
        }

        if ($('input[name="entity_abuse_comment_enabled"]:checked', context).length) {
          vals.push(Drupal.t('Abuses for comments: enabled', {}, {}));
        }
        else {
          vals.push(Drupal.t('Abuses for comments: disabled', {}, {}));
        }

        return Drupal.checkPlain(vals.join(', '));
      });
    }
  };

})(jQuery);
