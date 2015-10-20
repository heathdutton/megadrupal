/**
 * @file Javascript for batch jobs.
 */

(function ($) {

  Drupal.behaviors.batch_jobs = {
    attach: function (context, settings) {
      var batch = $('div.batch').attr('class');
      if (typeof batch !== 'undefined') {
        batch = batch.split(' ');
        var bid = batch[1];
        var bid = bid.replace(/^batch-/, '');
        token = batch[2];
        var token = token.replace(/^batch-/, '');
        var progress = 0;
        $('#progress').progressbar({
          value: progress
        });
        $('div.batch-percent').html(progress + '%');
        $.get(Drupal.settings.basePath + 'batch_jobs/' + bid + '/callback/' +
          token, null, updateProgressBar);
      }

      $('table').find('button.run').click(function(event) {
        if ($(event.target).html() == 'Run') {
          var task = $(event.target).attr('class');
          task = task.split(' ');
          var task = task[1];
          var token = $('div.tasks').attr('class');
          token = token.split(' ');
          var token = token[1];
          $(event.target).html('Running');
          $.get(Drupal.settings.basePath + 'batch_jobs/' + task + '/task/' +
            token, null, updateTask);
        }
        return false;
      });
    }
  };

  var updateProgressBar = function(response) {
    if (response.status) {
      var progress = Math.round(10000.0 * (response.complete /
        response.total)) / 100.0;
      $('#progress').progressbar({
        value: progress
      });
      $('div.batch-complete').html(response.complete + ' of ' + response.total);
      $('div.batch-percent').html(progress + '%');
      if (response.complete == response.total) {
        $.get(Drupal.settings.basePath + 'batch_jobs/' + response.bid +
          '/finished/' + response.token, null, jobFinished);
      }
      else {
        $.get(Drupal.settings.basePath + 'batch_jobs/' + response.bid +
          '/callback/' + response.token, null, updateProgressBar);
      }
    }
  };

  var jobFinished = function(response) {
    $('div.batch-complete').append('<p><a href ="' + Drupal.settings.basePath +
      'admin/reports/batch_jobs">Batch jobs</a></p>');
  };

  var updateTask = function(response) {
    var tr = $('table').find('button.' + response.tid).parent().parent();
    var td = $(tr).find('td').first();
    $(td).next().next().html(response.start);
    $(td).next().next().next().html(response.end);
    $(td).next().next().next().next().html(response.status);
    $(td).next().next().next().next().next().html(response.message);
    $(td).next().next().next().next().next().next().html('');
  };
})(jQuery);
