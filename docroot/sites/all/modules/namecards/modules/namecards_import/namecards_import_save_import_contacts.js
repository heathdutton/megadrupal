Drupal.behaviors.progresbardemo = function (context) {
  $('#progress').each(function () {
    var holder = this;

    // Send ajax request to start new node save process.
    $.ajax({
      type: 'POST',
      url: '/namecards/import_batch_save_nodes', // Which url should be handle the ajax request. This is the url defined in the <a> html tag
      success: function() {}, // The js function that will be called upon success request
      error: function() {
        alert('An error has occurred during the ajax call "namecards/import_batch_save_nodes".');
      },
      dataType: 'json', //define the type of data that is going to get back from the server
      data: 'js=1' //Pass a key/value pair
    });
    
    var updateCallback = function (progress, status, pb) {
      if (progress == 100) {
        pb.stopMonitoring();
        // Clean up session variables on the sever.
        $.ajax({
          type: 'POST',
          url: '/namecards/import_save_new_contacts_post_save', // Which url should be handle the ajax request. This is the url defined in the <a> html tag
          //success: function() {}, // The js function that will be called upon success request
          /*error: function() {
            alert('An error has occurred during the ajax call "/namecards/import_save_new_contacts_post_save".');
          },*/
          dataType: 'json', //define the type of data that is going to get back from the server
          data: 'js=1' //Pass a key/value pair
        });
        window.location = '/namecards/settings';
      }
    };

    var progress = new Drupal.progressBar('node_import', updateCallback, "POST");
    $(holder).append(progress.element);
    progress.startMonitoring('/namecards/import_save_new_contacts_progress_monitor', 2000);
  });
};