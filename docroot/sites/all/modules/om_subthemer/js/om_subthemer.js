$(document).ready(function() { 
  /*
  $("table#om-subthemer-layout td.col-tagid input.form-text").change(function() { 
    var val = $(this).parent().parent().parent().attr('id'); 
    //alert(val);
    $('#' + val + ' td.col-update input.form-checkbox').attr('checked', 'checked')
  }); 
  $("table#om-subthemer-layout td.col-type select").change(function() { 
    var val = $(this).parent().parent().parent().attr('id'); 
    //alert(val);
    $('#' + val + ' td.col-update input.form-checkbox').attr('checked', 'checked')
  }); 
  $("table#om-subthemer-layout td.col-iw input.form-checkbox").change(function() { 
    var val = $(this).parent().parent().parent().parent().attr('id'); 
    //alert(val);
    $('#' + val + ' td.col-update input.form-checkbox').attr('checked', 'checked')
  }); 
  $("table#om-subthemer-layout td.col-tagid a.tabledrag-handle").click(function() { 
    var val = $(this).parent().parent().attr('id'); 
    //alert(val);
    $('#' + val + ' td.col-update input.form-checkbox').attr('checked', 'checked')
  });  
  */  

  // make all siblings columns or rows
  
  
  // currently variable shouldn't have inner divs set on layout
  $("table#om-subthemer-layout td.col-type select").change(function() { 
    var rowID = $(this).parent().parent().parent().attr('id'); 
    var val = $(this).val(); 
    var parentID = $('#' + rowID + ' td.col-hidden input.om-pid').val();
    //alert(rowID);
    //alert(parentID);

    
    if(val == 'variable') {
      $('#' + rowID + ' td.col-iw input.form-checkbox').attr('disabled', 'disabled');
      $(this).parent().parent().removeClass('not-variable');
    }
    else {
      $('#' + rowID + ' td.col-iw input.form-checkbox').attr('disabled', '');
      $(this).parent().parent().addClass('not-variable');
      $('table#om-subthemer-layout tr.parent-' + parentID + ' td.col-type.not-variable select').val(val);
    }
  }); 
}); 


