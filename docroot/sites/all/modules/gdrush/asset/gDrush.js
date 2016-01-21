var path = require('path')
var gDrush = {
  gui : require('nw.gui'),
  outputTextArea: '',
  drupalPath: '',
  siteAlias:'',
  validSitePath: false,
  validDrupal: false,
  validDrushVersion: true,
  requriedDrushVersion: 6,
  sitesArray: Array(),
  includePath: path.dirname(window.location.pathname) + "/drush",
  modules: Array(),
  commandsQueue: Array(),
  commandRunning: false,
  options:{
    statusTable:'',
    modulesTable:'',
    sitesSelector:'',
    aliasSelector:'',
    panelsWrapper:'',
    tabsWrapper:'',
    outputArea:'',
    spinner:''
  },

  init: function (options, callback){
    if(options) gDrush.options = options;
    if(!gDrush.options) return;
    gDrush.updateSitesList(gDrush.options.sitesSelector);
    gDrush.options.panelsWrapper.html('');
    gDrush.options.tabsWrapper.html('');
    gDrush.drushQuery('version', [],function(error, version, stderr){
      version = version.split('.');
      version = parseInt(version[0]);
      if(version >= gDrush.requriedDrushVersion){  
        $('.main').show();
      }else{
        gDrush.validDrushVersion = false;
        $('.main').hide();
        var modal = $('#error-modal').clone();
        modal.find('.modal-title').text('Wrong drush version.');
        var msg = "<div>Drush version must be <b>" + gDrush.requriedDrushVersion + "</b> or greater your current drush version is <b>" + version + "</b></div>"
        msg += "<div>Please update your drush version</div>"
        modal.find('.modal-body').html(msg);
        modal.find('#ok').on('click',function(){
          gDrush.gui.App.quit();
        });
        modal.modal('show');
      }
    });
    gDrush.drushQuery('gdgp', [], function(error, panels, stderr){
      if(error === null){
        for(var key in panels){
          panel = panels[key];
          gDrush.options.panelsWrapper.append(gDrush.renderPanel(panel));
          gDrush.options.tabsWrapper.append('<li><a href="#' + panel.id + '-panel" data-toggle="tab">' + panel.title + '</a></li>');
        }
        gDrush.options.tabsWrapper.find('li:first a').tab('show');
        gDrush.changeSite(gDrush.options.sitesSelector.val(),gDrush.options.statusTable, gDrush.options.modulesTable);
      }
    });
    if(typeof callback == "function"){
      callback();
    }
  },

  renderPanel: function (panelInfo){
    var wrapper = $('<div>')
    var panel = $('<div id="' + panelInfo.id + '-panel" class="col-xs-7 panel panel-default tab-pane"></div>');
    panel.append('<div class="panel-heading">' + panelInfo.title + '</div>');
    panel.append('<div class="panel-body">' + panelInfo.html + '</div>');
    wrapper.append(panel);
    return wrapper.html();
  },

  changeSite: function (drupalPath, statusTable, modulesTable, updateAliasList){
    if(typeof statusTable == "string") statusTable = $(statusTable);
    if(updateAliasList == undefined) updateAliasList = true;
    gDrush.checkDrupalFolder(drupalPath,function(){
      gDrush.validDrupal = true;
      gDrush.drupalPath = drupalPath;
      gDrush.drushQuery('status', [], function(error, status, stderr){
        if(error === null){
          statusTable.find('tbody').html('');
          for(var key in status){
            if(typeof status[key] == "string"){
              var tr = '<tr><td>' + key + '</td><td>' + status[key] + '</td><tr>';
              statusTable.find('tbody').append(tr);
            }
          }
        }
        gDrush.updateModulesList(modulesTable);
        if(updateAliasList){
          gDrush.updateAliasList(gDrush.options.aliasSelector);
        }
      });
    },
    function(){
      gDrush.validDrupal = false;
    }); 
  },

  changeAlias: function(siteAlias){
    if(!siteAlias) siteAlias = "@self";
    gDrush.siteAlias = siteAlias;
    gDrush.changeSite(gDrush.drupalPath, gDrush.options.statusTable, gDrush.options.modulesTable,false);
  },

  disableModule: function(module){
    if(!gDrush.modules[gDrush.drupalPath][module]) return;
    var module = gDrush.modules[gDrush.drupalPath][module];
    var modal = $('#yes-no-modal').clone();
    var msg = "There are no other modules requires this module."
    if(module['required_by'].length > 0){
      var msg = "<b>The following modules are going to be disabled:</b><div> " + module['required_by'].join(', ') + "</div>";
    }
    modal.find('.modal-title').text('Are you sure you want to disable ' + module['title']);
    modal.find('.modal-body').html(msg);
    modal.find('#no').text("Don't disable")
    modal.find('#yes').text("Disable")
    modal.find('#yes').attr('data-loading-text',"Disabling...")
    modal.find('#yes').on('click',function(){
      $(this).button('loading');
      gDrush.drushExec('dis', [module['extension'] , '-y'], function(error, stdout, stderr){
        if(error === null){
          gDrush.modules[gDrush.drupalPath][module['extension']]['status'] = 'disabled';
          if(module['required_by'].length > 0){
            for(var key in module['required_by']){
              if(gDrush.modules[gDrush.drupalPath][module['required_by'][key]]){
                gDrush.modules[gDrush.drupalPath][module['required_by'][key]]['status'] = "disabled"
              }
            }
          }
          gDrush.updateModulesList(gDrush.options.modulesTable);
          modal.modal('hide');
        }
      });
    });
    modal.on('hide.bs.modal', function (e) {
      $('.modal-backdrop.in').css('display','none');
    });
    modal.modal('show')
  },

  enableModule: function(module){
    if(!gDrush.modules[gDrush.drupalPath][module]) return;
    var module = gDrush.modules[gDrush.drupalPath][module];
    var modal = $('#yes-no-modal').clone();
    var msg = "This module is not dependent on any other modules."
    var download = Array();
    var enable = Array();
    if(module['requires'].length > 0){
      for(var key in module['requires']){
        if(gDrush.modules[gDrush.drupalPath][module['requires'][key]] && gDrush.modules[gDrush.drupalPath][module['requires'][key]]['status'] == "disabled"){
          enable.push(module['requires'][key]);
        }else if(!gDrush.modules[gDrush.drupalPath][module['requires'][key]]){
          download.push(module['requires'][key]);
        }
      }

      if(enable.length || download.length){
        msg = "";
      }

      if(!enable.length && !download.length){
        msg = "All required moduels are enabled.";
      }

      if(enable.length){
        msg += "<b>The following modules are going to be enabled:</b><div> " + enable.join(', ') + "</div>";
      }

      if(download.length){
        msg += "<b>The following modules are going to be downloaded and enabled:</b><div> " + download.join(', ') + "</div>";
      }

    }
    modal.find('.modal-title').text('Are you sure you want to enable ' + module['title']);
    modal.find('.modal-body').html(msg);
    modal.find('#no').text("Don't enable")
    modal.find('#yes').text("Enable")
    modal.find('#yes').attr('data-loading-text',"Enabling...")
    modal.find('#yes').on('click',function(){
      $(this).button('loading');
      gDrush.drushExec('en', [ module['extension'] , '-y' ], function(error, stdout, stderr){
        if(error === null){
          if(!download.length){
            gDrush.modules[gDrush.drupalPath][module['extension']]['status'] = 'enabled';
            if(module['requires'].length > 0){
              for(var key in module['requires']){
                if(gDrush.modules[gDrush.drupalPath][module['requires'][key]]){
                  gDrush.modules[gDrush.drupalPath][module['requires'][key]]['status'] = "enabled"
                }
              }
            }
            gDrush.updateModulesList(gDrush.options.modulesTable);
          }else{
            gDrush.updateModulesList(gDrush.options.modulesTable, true);
          }
          modal.modal('hide');
        }
      });
    });
    modal.on('hide.bs.modal', function (e) {
      $('.modal-backdrop.in').css('display','none');
    });
    modal.modal('show')
  },

  updateModulesList: function (modulesTable, rebuildCache){
    if(typeof modulesTable == "string") modulesTable = $(modulesTable);
    if(!gDrush.modules[gDrush.drupalPath] || rebuildCache){
      gDrush.drushQuery('pm-info', [], function(error, modules, stderr){
        if(error === null){
          modulesTable.find('tbody').html('');
          gDrush.modules[gDrush.drupalPath] = modules;
          for(var key in modules){
            var checked = "";
            if(modules[key]['status'] == "enabled") checked = "checked";
            var checkBox = "<input data-module='" + modules[key]['extension'] + "'type='checkbox' " + checked + "/>";

            var tr=$('<tr><td>' + modules[key]['title'] + '</td><td>' + checkBox + '</td><tr>');
            modulesTable.find('tbody').append(tr);
          }
        }
      });
    }else{
      modulesTable.find('tbody').html('');
      modules = gDrush.modules[gDrush.drupalPath]
      for(var key in modules){
        var checked = "";
        if(modules[key]['status'] == "enabled") checked = "checked";
        var checkBox = "<input data-module='" + modules[key]['extension'] + "'type='checkbox' " + checked + "/>";

        var tr=$('<tr><td>' + modules[key]['title'] + '</td><td>' + checkBox + '</td><tr>');
        modulesTable.find('tbody').append(tr);
      }
    }
  },

  updateSitesList: function (sitesSelector){
    if(typeof sitesSelector == "string") sitesSelector = $(sitesSelector);
    gDrush.sitesArray = localStorage.getObject('sites');
    if(!gDrush.sitesArray) gDrush.sitesArray = Array();
    sitesSelector.html('');
    for(var key in gDrush.sitesArray){
      var site = gDrush.sitesArray[key];
      var option = $('<option value="' + site.path + '">' + site.name + '</div>')
      sitesSelector.append(option);
    }
  },

  updateAliasList: function (aliasSelector){
    if(typeof aliasSelector == "string") aliasSelector = $(aliasSelector);
    aliasSelector.html('');

    gDrush.drushQuery('sa', [], function(error, aliases, stderr){
      if(error === null){
        var option = $('<option value="@self">No Alias</div>')
        aliasSelector.append(option);
        for(var key in aliases){
          if(!aliases[key]['site-list'] && key != "self" && key != "none"){
            var alias = aliases[key];
            var option = $('<option value="@' + key + '">' + alias['#name'] + '</div>')
            aliasSelector.append(option);
          }
        }
      }
    });
  },

  checkDrupalFolder: function (dir,onValid,onNotValid){
    gDrush.validSitePath = false;
    gDrush.drushQuery('status', [], function(error, status, stderr){

      if(error === null){
        if(status.bootstrap == "Successful"){
          gDrush.validSitePath = true;
          onValid();
          return;
        } 
      }
      onNotValid();
    }, dir)
  },

  drushQuery: function (cmd, args, callback, cwd, format){
    if(!format) format = 'json';
    gDrush.runCommand(cmd, args, callback, cwd, format);
  },

  drushExec: function (cmd, args, callback, cwd){
    gDrush.runCommand(cmd, args, callback, cwd)
  },

  runCommand: function (cmd, args, callback, cwd, format){
    var exec = require('child_process').exec;
    var child;
    var outformat = ""
    if(!gDrush.validDrushVersion && cmd != "version") return;
    if(!gDrush.commandRunning){
      gDrush. commandRunning = true;
      if((!gDrush.drupalPath || !gDrush.validDrupal ) && !(cwd && (cmd == 'status')) && (cmd != "gdgp") && (cmd != "version")) return;
      if(!args) args = "";
      if(!cwd) cwd = gDrush.drupalPath;
      if(format) outformat = "--format=" + format;
      if(typeof gDrush.options.spinner == "string") gDrush.options.spinner = $(gDrush.options.spinner)
      gDrush.options.spinner.css('display','block')
      child = exec("drush -i " + gDrush.includePath + " " + gDrush.siteAlias + " " + cmd + " " + args.join(" ") + " "+ outformat + " ", {'cwd':cwd}, function (error, stdout, stderr) {
        if (error !== null) {
          console.log('exec error: ' + error);
        }else{
          gDrush.options.outputArea.append('<tr><td><pre>' + stderr + '</pre></td></tr>'); 
          gDrush.options.outputArea.parents('.panel-body').scrollTop(gDrush.options.outputArea[0].scrollHeight);
        }
        if(typeof callback == "function"){
          if(format == 'json' && stdout) stdout = JSON.parse(stdout);
          callback(error,stdout,stderr);
        }
        gDrush.commandRunning = false;
        if(gDrush.commandsQueue.length > 0){
          command = gDrush.commandsQueue.shift();
          gDrush.runCommand(command.cmd, command.args, command.callback, command.cwd, command.format);
        }
        gDrush.options.spinner.css('display','none');
      });
    }else{
      var command = {cmd:cmd, args:args, callback: callback, cwd:cwd, format:format};
      gDrush.commandsQueue.push(command);
    }
  }
}