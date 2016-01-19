function showbigcalendar(id, calendarlink) {
  var xmlHttp;
  try {
    xmlHttp = new XMLHttpRequest();// Firefox, Opera 8.0+, Safari
  }
  catch (e) {
    try {
      xmlHttp = new ActiveXObject("Msxml2.XMLHTTP"); // Internet Explorer
    }
    catch (e) {
      try {
        xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
      }
      catch (e) {
        alert(Drupal.t("No AJAX!?"));
        return false;
      }
    }
  }

  xmlHttp.onreadystatechange = function () {
    if (xmlHttp.readyState == 4) {
      document.getElementById(id).innerHTML = xmlHttp.responseText;
    }
  }

  xmlHttp.open("GET", calendarlink, false);
  xmlHttp.send();
//alert(document.getElementById('days').parentNode.lastChild.childNodes[6].innerHTML);
//document.getElementById('days').parentNode.lastChild.childNodes[6].style.borderBottomRightRadius='<?php echo $border_radius ?>px';
//document.getElementById('days').parentNode.lastChild.childNodes[0].style.borderBottomLeftRadius='<?php echo $border_radius ?>px';	

  var thickDims, tbWidth, tbHeight;

  jQuery(document).ready(function ($) {

    thickDims = function () {
      var tbWindow = jQuery('#TB_window'), H = jQuery(window).height(), W = jQuery(window).width(), w, h;

      w = (tbWidth && tbWidth < W - 90) ? tbWidth : W - 200;
      h = (tbHeight && tbHeight < H - 60) ? tbHeight : H - 200;

      if (tbWindow.size()) {
        tbWindow.width(w).height(h);
        jQuery('#TB_iframeContent').width(w).height(h - 27);
        tbWindow.css({'margin-left':'-' + parseInt((w / 2), 10) + 'px'});
        if (typeof document.body.style.maxWidth != 'undefined')
          tbWindow.css({'top':(H - h) / 2, 'margin-top':'0'});
      }
    };

    thickDims();
    jQuery(window).resize(function () {
      thickDims()
    });

    jQuery('a.thickbox-preview' + id).click(function () {

      tb_click.call(this);

      var alink = jQuery(this).parents('.available-theme').find('.activatelink'), link = '', href = jQuery(this).attr('href'), url, text;

      if (tbWidth = href.match(/&tbWidth=[0-9]+/))
        tbWidth = parseInt(tbWidth[0].replace(/[^0-9]+/g, ''), 10);
      else
        tbWidth = jQuery(window).width() - 90;

      if (tbHeight = href.match(/&tbHeight=[0-9]+/))
        tbHeight = parseInt(tbHeight[0].replace(/[^0-9]+/g, ''), 10);
      else
        tbHeight = jQuery(window).height() - 60;

      if (alink.length) {
        url = alink.attr('href') || '';
        text = alink.attr('title') || '';
        link = '&nbsp; <a href="' + url + '" target="_top" class="tb-theme-preview-link">' + text + '</a>';
      } else {
        text = jQuery(this).attr('title') || '';
        link = '&nbsp; <span class="tb-theme-preview-link">' + text + '</span>';
      }

      jQuery('#TB_title').css({'background-color':'#222', 'color':'#dfdfdf'});
      jQuery('#TB_closeAjaxWindow').css({'float':'left'});
      jQuery('#TB_ajaxWindowTitle').css({'float':'right'}).html(link);

      jQuery('#TB_iframeContent').width('100%');
      thickDims();

      return false;
    });


  });

}
;


document.onkeydown = function (evt) {
  evt = evt || window.event;
  if (evt.keyCode == 27) {

    document.getElementById('sbox-window').close();

  }
}; 