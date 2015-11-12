/**
 * @file
 * Javascript for Workspace module.
 *
 */

(function($) {

/**
 * Function to send id of record to change status.
 *
 * @param value
 *   Value to be sent to server.
 * @param path
 *   path to callback.
 *
 */
  Drupal.workspace_json_status_switch = function(value, path) {
    $.get("http://" + Drupal.settings.workspace.host + Drupal.settings.basePath + path,
      { value: value },
      function(data) {
        if (data['error'] != undefined && data['error'] != '') {
          alert(data['error']);
        }
        else {
          var stat = data['status'];
          if (Drupal.settings.workspace.use_icons) {
            imgpath = Drupal.settings.basePath + Drupal.settings.workspace.modulepath + '/images/' + stat + '.png';
            $("img#workspace_switch_"+data['id']).attr({
              alt:Drupal.settings.workspace[stat],
              title:Drupal.settings.workspace[stat],
              src:imgpath
            });
          }
          else {
            // no icons
            $("#workspace_switch_"+data['id']).attr({
              title:Drupal.settings.workspace[stat],
            });
            $("#workspace_switch_"+data['id']).html(Drupal.settings.workspace[stat]);
          }
        }
      },
      "json"
    );
  }

})(jQuery);
