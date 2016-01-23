(function ($) {

/**
 * Provide the summary information for the block settings vertical tabs.
 */
Drupal.behaviors.intel_disqus = {
  attach: function (context) {
	_l10iq.push(['_registerCallback', 'saveCommentSubmitPostSubmit', this.saveCommentSubmitPostCallbackSubmit, this]);
  },

  saveCommentSubmitPostCallbackSubmit: function (data) {
    if (data.type != 'disqus') {
      return;
    }
    var data = {
      vtk: _l10iq.vtk,
      commentid: data.commentid
    };
    var url = ('https:' == document.location.protocol) ? 'https://' : 'http://';
    url += Drupal.settings.intel.cms_hostpath + "intel_disqus/comment_submit_js";
    jQuery.ajax({
      dataType: 'json',
      url: url, 
      data: data,
      type: 'GET'
    });
  }
};

})(jQuery);