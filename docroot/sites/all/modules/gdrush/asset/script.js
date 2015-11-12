

Storage.prototype.setObject = function(key, value) {
    this.setItem(key, JSON.stringify(value));
}

Storage.prototype.getObject = function(key) {
    var value = this.getItem(key);
    return value && JSON.parse(value);
}

document.oncontextmenu = function(event){
  if(event.target.nodeName == "INPUT"){
    return true;
  }
  return false;
}

$(function(){
  var fs   = require('fs'),
      path = require('path'),
      gui = require('nw.gui');

  gui.Window.get().show();
  gDrush.init({ 
    statusTable:'#status-table', 
    modulesTable: '#modules-table', 
    tabsWrapper:$('.tabs-wrapper'), 
    panelsWrapper:$('.panels-wrapper'), 
    sitesSelector:$('#sites-selector'), 
    aliasSelector:$('#alias-selector'), 
    outputArea:$('#output-area tbody'),
    spinner:$('#cmd-spinner')
  });


  $('.navbar-nav li').on('mousedown',function(){
    $(this).addClass('active');
  })
  .on('mouseup',function(){
    $(this).removeClass('active');
  })

  $(document).on('click',"div[data-cmd-group] button",function(){
    var cmd = $(this).parent('div[data-cmd-group]').attr("data-cmd-group") + " " + $(this).attr('data-cmd')
    gDrush.drushExec(cmd,[]);
  });

  $(document).on('click','input[data-module]',function(){
    if( !$(this).is(':checked') ) gDrush.disableModule($(this).attr('data-module'));
    if( $(this).is(':checked') ) gDrush.enableModule($(this).attr('data-module'));
  })

  $('.modal').on('hide.bs.modal', function (e) {
    $('.modal-backdrop.in').css('display','none');
  });

  $('#site-name').keyup(function(){
    validateAddSite();
  });

  $('#add-new-site').on('click',function(){
    if(validateAddSite){
      var info = {name:$('#site-name').val(), path:$('#site-path').val()};
      gDrush.sitesArray.push(info);
      localStorage.setObject('sites',sitesArray);
      gDrush.updateSitesList();
      $('#add-site-modal').modal('hide');
    }
  });

  $('#sites-selector').change(function(){
    gDrush.changeSite($(this).val(), $('#status-table'), $('#modules-table'));
  });

  $('#alias-selector').change(function(){
    gDrush.changeAlias($(this).val());
  });

  $('button[data-file]').on('click',function(){
    var chooser = $("#"+$(this).attr('data-file'))
    chooser.change(function(evt) {
      $('#site-path').val($(this).val());
      gDrush.checkDrupalFolder($(this).val(),function(){validateAddSite(); },function(){validateAddSite();});
    });

    chooser.trigger('click');
  })

});

function validateAddSite (){
  if(gDrush.validSitePath && $('#site-name').val() != ""){
    $('#add-new-site').removeAttr('disabled')
  }else{
    $('#add-new-site').attr('disabled','');
  }
}
