/**
 * @file
 * JavaScript tweaks to the Preview Sync screens.
 */
(function ($, Drupal, window, document, undefined) {

/**
 * Run when the DOM is ready.
 */
Drupal.behaviors.previewSync = {
  attach: function (context, settings) {
    // Ensure the tasks fit across the page nicely.
    if ($('.table-tasks', context).length) {
      var $tasks = $('.table-tasks .task', context);
      var width = 100 / $tasks.length;
      $tasks.css('width', width + '%');

      // Also start the auto-updater of the tasks being coloured in, so the user
      // knows roughly what is happening. This only happens when the
      var refresh = setInterval(function() {
        previewSyncUpdate();
      }, 5 * 1000);
    }
  }
};

/**
 * Main loop function that controls polling.
 */
function previewSyncUpdate() {

  $.getJSON(Drupal.settings.basePath + Drupal.settings.pathPrefix + 'ajax/preview_sync', function(data) {

    // Update the elapsed time.
    $('.table-tasks .elapsed-time').html(data.elapsed_time);

    // If there are no tasks, then reload the page.
    if (data.tasks.length === 0) {
      location.href = location.href;
    }

    // // Loop through the tasks and update them.
    $.each(data.tasks, function(id, task) {
      // Update the status.
      var $task = $('.table-tasks .task[data-item-id="' + id + '"]');
      var $box = $task.find('.box');

      if (task.duration) {
        $box.find('span').html(task.duration_friendly);
      }

      // Remove all classes to start with.
      $box.removeClass('not-started')
          .removeClass('success')
          .removeClass('failure')
          .removeClass('in-progress');

      // Add in the current status class.
      switch (task.status) {
        case -1 :
          $box.addClass('not-started');
          break;
        case 0 :
          $box.addClass('failure');
          break;
        case 1 :
          $box.addClass('success');
          break;
        case 2 :
          $box.addClass('in-progress');
          if (task.remaining_friendly) {
            $('.table-tasks .remaining-time').html(task.remaining_friendly);
          }
          break;
        default :
          console.log('Whoops, missing status here - ' + task.status);
          break;
      }
    });

  });
}

})(jQuery, Drupal, this, this.document);
