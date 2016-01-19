(function ($) {
  var $window = $(window);

  /**
   * Our dialog object. Can be used to open a dialog to anywhere.
   */
  Drupal.GanttDialog = {
    dialog_open: false,
    open_dialog: null
  }

  /**
   * Open a dialog window.
   * @param string href the link to point to.
   */
  Drupal.GanttDialog.open = function (href, title, parent, project) {
    if (!this.dialog_open) {
      // Add render gantt dialog, so that we know that we should be in a
      // dialog.
      href += (href.indexOf('?') > -1 ? '&' : '?') + 'render=gantt-dialog&parent=' + parent + '&project=' + project;
      // Get the current window size and do 70% of the width and 85% of the height.
      var window_width = $window.width() / 100 * 70;
      var window_height = $window.height() / 100 * 85;
      this.open_dialog = $('<iframe class="gantt-dialog-iframe" src="' + href + '"></iframe>').dialog({
        width: window_width,
        height: window_height,
        modal: true,
        resizable: false,
        position: ['center', 50],
        title: title,
        close: function () { 
          Drupal.GanttDialog.dialog_open = false;
          $('.gantt-dialog-iframe').remove();
          refreshGantt();
        }
      }).width(window_width - 10).height(window_height);

      $window.bind('resize scroll', function () {
        // Move the dialog the main window moves.
        if (typeof Drupal.GanttDialog == 'object' && Drupal.GanttDialog.open_dialog != null) {
          Drupal.GanttDialog.open_dialog.dialog('option', 'position', ['center', 10]);
          Drupal.GanttDialog.setDimensions();
        }
      });
      this.dialog_open = true;
    }
  }

  /**
   * Set dimensions of the dialog dependning on the current window size
   * and scroll position.
   */
  Drupal.GanttDialog.setDimensions = function () {
    if (typeof Drupal.GanttDialog == 'object') {
      var window_width = $window.width() / 100 * 70;
      var window_height = $window.height() / 100 * 85;
      this.open_dialog.
        dialog('option', 'width', window_width).
        dialog('option', 'height', window_height).
        width(window_width - 10).height(window_height);
    }
  }

  /**
   * Close the dialog and provide an entity id and a title
   * that we can use in various ways.
   */
  Drupal.GanttDialog.close = function (entity_type, entity_id, title) {
    this.open_dialog.dialog('close');
    this.open_dialog.dialog('destroy');
    this.open_dialog = null;
    this.dialog_open = false;

    refreshGantt();
  }

  Drupal.behaviors.views_gantt = {
    attach: function (context) {
      if ( $('.view-content .gantt-wrapper.full').length > 0) {
        if ( $('body > .gantt-wrapper.full').length > 0) {
          $('body > .gantt-wrapper.full').remove();
        } 
        $('.gantt-wrapper.full')
        .appendTo('body')
        .css('height', $(window).height()-50);
      }
      gantt.config.xml_date = "%Y-%m-%d %H:%i";
      gantt.config.details_on_dblclick = false;
      gantt.config.links = {"finish_to_start" : "0"};
      // Add Task name column.
      gantt.config.columns = [
        {name:"text", label:"Task name", tree:true, width:'*' },
      ];
      // Add aditional columns from gantt views style settings.
      $.each(Drupal.settings.views_gantt.columns, function(key, val) {
        column = {name:key, label: val, align: 'center'};
        if (key == 'progress') {
          column.template = function(obj) {
            return Math.round(obj.progress * 100) + "%"
          }
        }
        gantt.config.columns.push(column);
      });
      // Add column 'Add new Task'.
      if (Drupal.settings.views_gantt.add_task) {
        column = {name:"add", label:"", width:44};
        gantt.config.columns.push(column);
      }
      // Add type name to css classes line for tasks and project.
      gantt.templates.task_class = function(start, end, obj){
        return obj.type;
      }

      var value = $.cookie("ganttScale");
      if (value == null) {
        value = Drupal.settings.views_gantt.scale;
      }
      $('input[name="scale"][value=' + value + ']').attr('checked', 'checked');
      $('input[name="scale2"][value=' + value + ']').attr('checked', 'checked');

      setScaleConfig(value);

      if ($('body').hasClass('views-gantt-full')){
        gantt.init("GanttDivFullscreen");
        $('.gantt-wrapper.full').show();
      }
      else{
        gantt.init("GanttDiv");
      }

      gantt.clearAll(); 
      gantt.load(ganttUrl(), "json");
 
      // Set click event for scaling inputs.
      $('input[name="scale"]').live('click', function(e) {
        e = e || window.event;
        var el = e.target || e.srcElement;
        var value = el.value;
        $.cookie("ganttScale", value);
        $('input[name="scale2"][value=' + value + ']').attr('checked', 'checked');
        setScaleConfig(value);
        gantt.render();
      });

      $('input[name="scale2"]').live('click', function(e) {
        e = e || window.event;
        var el = e.target || e.srcElement;
        var value = el.value;
        $.cookie("ganttScale", value);
        $('input[name="scale"][value=' + value + ']').attr('checked', 'checked');
        setScaleConfig(value);
        gantt.render();
      });

      var dp = new dataProcessor(Drupal.settings.basePath + 'views_gantt/update');
      dp.init(gantt);
      $('body',context).once(function() {
        // Update chart after dragging.
        gantt.attachEvent("onAfterTaskUpdate", function(id, item) {
          if (item['parent']) {
            progressUpdate(id, gantt, dp);
          }
        });

        // Task click event, show custom dialog.
        gantt.attachEvent("onTaskDblClick", function(id,e){
          if (!Drupal.settings.views_gantt.edit_task) {
            return false;
          }

          if (id == null) {
            return false;
          }

          task = gantt.getTask(id);
          if (task['type'] == 'project') {
            return false;       
          } 
          else {
            Drupal.GanttDialog.open(
              Drupal.settings.basePath + 'node/' + id + '/edit', 
              task['text'], 
              task.parent, Drupal.settings.views_gantt.project_id
            );

            return false;
          }
        });

        // Disable click event for header add column.
        gantt.attachEvent("onGridHeaderClick", function(name, e){
          if (name == 'add') {
            return false;
          }
        });

        // Fullscreen click event.
        $('.gantt-fullscreen').live('click', function(e) {
          toggleFullScreen($(this).data('mode'));
          e.preventDefault();
        });

        gantt.attachEvent("onBeforeLinkAdd", function(id,item){
          item.id = item.source + '_' + item.target;
        });
        // Disable lightbox for add task, add custom dialog.
        gantt.attachEvent("onBeforeLightbox", function(id){
          var task = gantt.getTask(id);
          if (task == null) return false;

          if(task.$new){
            if (!Drupal.settings.views_gantt.add_task) {
              gantt.deleteTask(task.id);
              return false;
            }
            Drupal.GanttDialog.open(
              Drupal.settings.basePath + 'node/add/' + Drupal.settings.views_gantt.task_type, 
              task['text'], 
              task.parent, Drupal.settings.views_gantt.project_id
            );
          
            gantt.deleteTask(task.id);
              
            return false;
          }
          return true;
        });
      });
    }
  }

  function refreshGantt() {
    if (Drupal.settings && Drupal.settings.views_gantt && Drupal.settings.views_gantt.view) {
      var settings = Drupal.settings.views_gantt.view.settings;

      // Search in settings view which need update.
      if (settings.view_name && settings.view_display_id) {
        var selector = '.view-dom-id-' + settings.view_dom_id,
            href = Drupal.settings.views_gantt.view.href,
            viewData = {};

        // Construct an object using the settings defaults and then overriding
        // with data specific to the link.
        $.extend(
          viewData,
          settings,
          Drupal.Views.parseQueryString(href),
          // Extract argument data from the URL.
          Drupal.Views.parseViewArgs(href, settings.view_base_path)
        );

        // For anchor tags, these will go to the target of the anchor rather
        // than the usual location.
        $.extend(viewData, Drupal.Views.parseViewArgs(href, settings.view_base_path));

        // Ajax settings for uping view.
        var ajax_settings = {
          url: Drupal.settings.views_gantt.ajax_path,
          submit: viewData,
          selector: selector
        };

        var ajax = new Drupal.ajax(false, false, ajax_settings);
        ajax.eventResponse(ajax, {});
      }
    }
  }

  function setScaleConfig(value){
    switch (value) {
      case "1":
        gantt.config.scale_unit = "day"; 
        gantt.config.date_scale = "%d %M"; 

        gantt.config.scale_height = 60;
        gantt.config.min_column_width = 30;
        gantt.config.subscales = [
              {unit:"hour", step:1, date:"%H"}
        ];
        break;

      case "2":
        gantt.config.min_column_width = 70;
        gantt.config.scale_unit = "day"; 
        gantt.config.date_scale = "%d %M"; 
        gantt.config.subscales = [ ];
        gantt.config.scale_height = 35;
        break;

      case "3":
        gantt.config.min_column_width = 70;
        gantt.config.scale_unit = "month"; 
        gantt.config.date_scale = "%M"; 
        gantt.config.scale_height = 60;
        gantt.config.subscales = [
              {unit:"week", step:1, date:"#%W"}
        ];
        break;
    }
  }

  function toggleFullScreen(active_mode) {
    disabled_mode = active_mode == 'default' ? 'full' : 'default';
    active_selector = '.gantt-wrapper.' + active_mode;
    active_id = $(active_selector).find('.gantt-div').attr('id');
    disabled_selector = '.gantt-wrapper.' + disabled_mode;
    disabled_id = $(disabled_selector).find('.gantt-div').attr('id');    
    $('body > *').removeClass('views-gantt-hidden');
    if (active_mode == 'full') {
      if ( $('.view-content .gantt-wrapper.full').length > 0) {
        if ( $('body > .gantt-wrapper.full').length > 0) {
          $('body > .gantt-wrapper.full').remove();
        }
        $('.gantt-wrapper.full')
        .appendTo('body')
        .css('height', $('#' + active_id).offsetHeight+'px');
      }

      $('body').addClass('views-gantt-full');
      $('body > *:not(.dhx_modal_cover):visible').addClass('views-gantt-hidden');
    }
    else {
      $('body').removeClass('views-gantt-full');
      $('body > *').removeClass('views-gantt-hidden');
    }

    $(active_selector).show();
    $(disabled_selector).hide();

    gantt.init(active_id);
  }

  function progressUpdate(id, gantt, dp) {
    task = gantt.getTask(id);

    children_hours_completed = 0;
    children = gantt.getChildren(id);
    if (children.length) {
      $.each(children, function(key, child_id) {
        child = gantt.getTask(child_id);
        progress = child.progress;
        duration = child.duration;
        hours_completed = duration * progress;
        children_hours_completed += hours_completed;
      });
      task.progress = children_hours_completed / task.duration;
      if (task.progress > 1) task.progress = 1;
      gantt.refreshTask(id);
      dp.setUpdated(id, true);
    }

    if (task['parent']) {
      progressUpdate(task['parent'], gantt, dp);
    }
  }

  function ganttUrl() {
    url = Drupal.settings.basePath + 'views_gantt/data.json?' + 
      'view=' + Drupal.settings.views_gantt.view_name + 
      '&display=' + Drupal.settings.views_gantt.display_id + 
      '&project=' + Drupal.settings.views_gantt.project_id; 

    if (Drupal.settings.views_gantt.exposed_input) {
      $.each(Drupal.settings.views_gantt.exposed_input, function(key, val) {
        if (val) {
          url += '&' + key + '=' + val;
        }
      });
    }

    return url;   
  }
})(jQuery);
