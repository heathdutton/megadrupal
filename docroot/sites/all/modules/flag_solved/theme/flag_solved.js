$(document).bind('flagGlobalAfterLinkUpdate', function(event, data) {
  function commentExistsOnPage(type, id) {
    var domId = type + '-' + id;
    return $('#'+domId).length != 0;
  }

  function refreshComment(type, id, status) {
    var domId = type + '-' + id;
    if (status == 'flagged') {
      $('#'+domId).addClass('solved');
    }
    else {
      $('#'+domId).removeClass('solved');
    }
  }

  if (data.flagName == 'solved_comment') {
    if (commentExistsOnPage('reply', data.contentId)) {
      refreshComment('reply', data.contentId, data.flagStatus);
    }
  }
  else if (data.flagName == 'solved_node') {
    if (commentExistsOnPage('node', data.contentId)) {
      refreshComment('node', data.contentId, data.flagStatus);
    }
  }
});
