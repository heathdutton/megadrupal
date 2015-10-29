(function($) {
$.webcam = function() {
  return this;
};
$.webcam.compareBaseUrls = function(url1, url2) {
  if (typeof(url1 == 'String') && url1.indexOf('?') !== -1) {
    url1 = url1.substr(0, url1.indexOf('?'));
  }
  if (typeof(url2 == 'String') && url2.indexOf('?') !== -1) {
    url2 = url2.substr(0, url2.indexOf('?'));
  }
  if (url1 == url2) {
    return true;
  } else {
    return false;
  }
};
$.webcam.counters = new Array();
$.webcam.timers = new Array();
$.webcam.errors = false;
$.webcam.init = function(id, name, url, durl, width, height, time, timeout, errid, errmsg){
  $.webcam.timers[id] = setInterval("$.webcam.refresh('" + id + "', '" + name + "', '" + url + "', '" + durl + "', " + width + ", " + height + ", " + timeout + ", '" + errid + "', '" + errmsg + "')", time);
  $.webcam.counters[id] = 0;
  $.webcam.refresh(id, name, url, durl, width, height, timeout, errid, errmsg);
};
$.webcam.refresh = function(id, name, url, durl, width, height, timeout, errid, errmsg){
  var theDate = new Date();
  if (timeout > 0 && $.webcam.counters[id]++ > timeout) {
    clearInterval($.webcam.timers[id]);
    if ($.webcam.errors == false) {
      $('#'+errid).prepend('<div class="error"></div>');
      $.webcam.errors = true;
    }
    $('#'+errid+' .error').append(errmsg + '<br />');

    // Add some code for to integrate webcams with thickbox
    if (typeof tb_show == 'function' && $('#'+id+' a.thickbox').attr('href') !== undefined) {
      if ($.webcam.compareBaseUrls(url, $('#'+id+' a.thickbox').attr('href')) || $.webcam.compareBaseUrls(durl, $('#'+id+' a.thickbox').attr('href'))) {
        $('#'+id+' a.thickbox').unbind('click');
        $('#'+id+' a.thickbox').click(function(){
          var t = this.title || this.name || null;
          var a = this.href || this.alt;
          var g = this.rel || false;
          tb_show(t,a,g);
          $('#TB_window').prepend('<div id="TB_error"><div class="error">' + errmsg + '</div></div>');
          $("#TB_load").remove();
          this.blur();
          return false;
        });
      }
      if (document.getElementById("TB_window") !== null) {
        if ($.webcam.compareBaseUrls(url, $('#TB_Image').attr('src'))) {
          $('#TB_window').prepend('<div id="TB_error"><div class="error">' + errmsg + '</div></div>');
        }
      }
    }
  } else {
    $.webcam.preload(id, url + "?" + parseInt(theDate.getTime() / 1000), durl, width, height);
  }
};
$.webcam.preload = function(id, url, durl, width, height){
  var img = new Image();
  img.onerror = function(){
    $('#'+id+' img.webcams-image').attr('src', durl);
  };
  img.onload = function(){
    img.onload = null;
    if (img.width > width) {
      img.height = img.height * (width / img.width);
      img.width = width;
      if (img.height > height) {
        img.width = img.width * (height / img.height);
        img.height = height;
      }
    } else if (img.height > height) {
      img.width = img.width * (height / img.height);
      img.height = height;
      if (img.width > width) {
        img.height = img.height * (slideW / img.width);
        img.width = width;
      }
    }
    $('#'+id+' img.webcams-image').attr('src', img.src).css('width', img.width).css('height', img.height);

    // Add some code for to integrate webcams with thickbox
    if (typeof tb_show == 'function' && $('#'+id+' a.thickbox').attr('href') !== undefined) {
      if ($.webcam.compareBaseUrls(url, $('#'+id+' a.thickbox').attr('href')) || $.webcam.compareBaseUrls(durl, $('#'+id+' a.thickbox').attr('href'))) {
        $('#'+id+' a.thickbox').attr('href', img.src);
      }
      if (document.getElementById("TB_Image") !== null) {
        if ($.webcam.compareBaseUrls(url, $('#TB_Image').attr('src'))) {
          $('#TB_Image').attr('src', img.src);
        }
      }
    }
  };
  img.src = url;
};
})(jQuery);
