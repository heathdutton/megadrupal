
(function ($) {

Drupal.whiteboard = Drupal.whiteboard || {'initialised' : false};

Drupal.behaviors.whiteboardBehavior = {
attach: function (context, settings) {
if($('#whiteboard').length)
{
  var system = 'default' /* Type of grid */
  var saved = Drupal.t('Saved.');
  var saveFailed = Drupal.t('Changes not saved. Check permissions and filters');
  var hiddenEllipseForm = Drupal.t('Ellipse drawn');
  var cancelEllipseForm = Drupal.t('Ellipse cancelled.');
  var displayEllipseForm = Drupal.t('Enter ellipse specifications.');
  var hiddenRectangleForm = Drupal.t('Rectangle drawn');
  var cancelRectangleForm = Drupal.t('Rectangle cancelled.');
  var displayRectangleForm = Drupal.t('Enter rectangle specifications.');
  var hiddenCircleForm = Drupal.t('Circle drawn');
  var cancelCircleForm = Drupal.t('Circle cancelled.');
  var drawBezierCurve = Drupal.t('Bezier curve drawn.');
  var displayCircleForm = Drupal.t('Enter circle specifications.');
  var hiddenArcForm = Drupal.t('Arc drawn');
  var cancelArcForm = Drupal.t('Arc cancelled.');
  var displayArcForm = Drupal.t('Enter arc specifications.');
  var penColorSaved = Drupal.t('Pen color set.');
  var penWidthSaved = Drupal.t('Pen width set.');
  var pointsCleared = Drupal.t('Points Cleared.');
  var whiteboardLoaded = Drupal.t('Whiteboard loaded.');
  var setCartecianMsg = Drupal.t('Coordinate System set to Cartesian.');
  var setDefaultMsg = Drupal.t('Coordinate System set to Default.');
  var setRangeMsg = Drupal.t('Range is displayed.');
  var drawLineMsg = Drupal.t('Line drawn.');
  var drawPolyline = Drupal.t('Polyline drawn.');
  var drawPolygon = Drupal.t('Polygon drawn.');
  var drawCurve = Drupal.t('Curve drawn.');
  var drawClosedCurve = Drupal.t('Closed curve drawn.');
  var unsetRangeMsg = Drupal.t('Range is hidden.');
  var showGridMsg = Drupal.t('Showing Grid.');
  var hideGridMsg = Drupal.t('Grid is hidden.');
  var whiteboardCleared = Drupal.t('Whiteboard cleared.');
  var eraserOn = Drupal.t('Eraser On.');
  var whiteboard = document.getElementById('whiteboard');
  var col = new jsColor('#99BBFF');
  var pen = new jsPen(col, 1);
  var gr=new jsGraphics(whiteboard);
  var points = new Array();
  var ie = false; /* Variable to hold browser type */
  var mouseX = 0;
  var mouseY = 0;
  var offset = $('#whiteboard').offset(); /* To center coordinates of mouse clicks */
  var mouseDown = false;
  var gridColor = new jsColor('#EEEEEE');
  if($('#whiteboard').length)
  {
    var wbWidth = Number($('#whiteboard').css('width').replace('px', ''));
    var wbHeight = Number($('#whiteboard').css('height').replace('px', ''));
  }
  var x = wbWidth/ 2;
  var y = wbHeight / 2;
  var cartecianOrigin = new jsPoint(x, y);
  var defaultOrigin = new jsPoint(0, 0);

  function setPenColor() {
    if ($('#color').val() != '')
      col = new jsColor($('#color').val());
    else
      col = new jsColor("#99BBFF");
    if ($('#penwidth').val() != '') {
      penWidth = $('#penwidth').val();
      pen = new jsPen(col, penWidth);
    }
    else
      pen = new jsPen(col, 1);
  }

  function drawPoint() {
    gr.fillRectangle(col, new jsPoint(mouseX,mouseY),1,1);
    points[points.length]=new jsPoint(mouseX,mouseY);
    point=new jsPoint(mouseX, mouseY);
  }

  function drawLine() {
    gr.drawLine(pen,points[points.length-2],points[points.length-1]);
  }

  function whiteboardClearBoard() {
    var div = $('#whiteboard div');
    var img =  $('#whiteboard img');
    for(j = 0; j < div.length; j++) {
        if (((div[j].style.cssText.search('#') > -1) || (div[j].style.cssText.search('rgb') > -1)) && div[j].parentNode != null ) {
          div[j].parentNode.removeChild(div[j]);
        }
    }
    for(i = 0; i < img.length; i++) {
      img[i].parentNode.removeChild(img[i]);
    }
  } 

  $('#edit-whiteboard-clear-points').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    points = new Array();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(pointsCleared).fadeIn(100);});
  });

  $('#edit-whiteboard-clear-board').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    whiteboardClearBoard();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(whiteboardCleared).fadeIn(100);});
  });

   $('#edit-whiteboard-submit').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
  });

  $('#edit-whiteboard-penwidth-fine').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#penwidth').val('1');
    setPenColor();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(penWidthSaved).fadeIn(100);});
  });

  $('#edit-whiteboard-penwidth-thick').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#penwidth').val('7');
    setPenColor();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(penWidthSaved).fadeIn(100);});
  });

  $('#edit-whiteboard-penwidth-blunt').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#penwidth').val('15');
    setPenColor();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(penWidthSaved).fadeIn(100);});
  });

  $('#edit-whiteboard-black').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#color').val('#777777');
    setPenColor();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(penColorSaved).fadeIn(100);});
  });

  $('#edit-whiteboard-blue').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#color').val('#99BBFF');
    setPenColor();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(penColorSaved).fadeIn(100);});
  });

  $('#edit-whiteboard-green').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#color').val('#99FF99');
    setPenColor();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(penColorSaved).fadeIn(100);});
  });

  $('#edit-whiteboard-red').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#color').val('#FF9999');
    setPenColor();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(penColorSaved).fadeIn(100);});
  });

  $('#edit-whiteboard-eraser').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#color').val('#FFFFFF');
    setPenColor();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(eraserOn).fadeIn(100);});
  });

  $('#edit-whiteboard-hex').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#whiteboard-color-container').show(100);
  });

  $('#edit-whiteboard-color-cancel').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#whiteboard-color-container').hide(100);
  });
  $('#edit-whiteboard-color-submit').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#color').val( $('#edit-whiteboard-color-code').val() );
    setPenColor();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(penColorSaved).fadeIn(100);});
    $('#whiteboard-color-container').hide(100);
  });

  function setCartecian() {
    gr.setCoordinateSystem('cartecian');
    gr.setOrigin(cartecianOrigin);
    gr.showGrid('50', false, gridColor);
    system = 'cartecian';
  }

  $('#edit-whiteboard-cartecian').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    setCartecian();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(setCartecianMsg).fadeIn(100);});
  });

  function setDefault() {
    gr.setCoordinateSystem('default');
    gr.setOrigin(defaultOrigin);
    gr.showGrid('50', false, gridColor);
    system = 'default';
  }
  
  $('#edit-whiteboard-default').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    setDefault();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(setDefaultMsg).fadeIn(100);});
  });

  function showRange() {
    gr.showGrid('50', true, gridColor);
  }

  $('#edit-whiteboard-show-range').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    showRange();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(setRangeMsg).fadeIn(100);});
  });

  function hideRange() {
    gr.showGrid('50', false, gridColor);
  }

  $('#edit-whiteboard-hide-range').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    hideRange();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(unsetRangeMsg).fadeIn(100);});
  });

  function showGrid() {
      gr.showGrid('50', false, gridColor);
  }

  $('#edit-whiteboard-show-grid').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    showGrid();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(showGridMsg).fadeIn(100);});
  });

  function hideGrid() {
    gr.hideGrid();
  }

  $('#edit-whiteboard-hide-grid').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    hideGrid();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(hideGridMsg).fadeIn(100);});
  });

  $('#edit-whiteboard-arc').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#whiteboard-arc').show(100);
    $('#whiteboard-arc-container form *').css('display', 'inline');
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(displayArcForm).fadeIn(100);});
  });

  $('#edit-whiteboard-arc-cancel').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#whiteboard-arc').hide(100);
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(cancelArcForm).fadeIn(100);});
  });

  $('#edit-whiteboard-arc-submit').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#whiteboard-arc').hide(100);
    drawArc();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(hiddenArcForm).fadeIn(100);});
  });

  function drawArc() {
    var width = $('#edit-whiteboard-arc-width').val()
    var height = $('#edit-whiteboard-arc-height').val();
    var startAngle = $('#edit-whiteboard-arc-angle').val();
    var swapAngle = $('#edit-whiteboard-arc-length').val();
    gr.drawArc(pen, point, width, height, startAngle, swapAngle);
    points = new Array();
  }

  $('#edit-whiteboard-rectangle').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#whiteboard-rectangle').show(100);
    $('#whiteboard-rectangle-container form *').css('display', 'inline');
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(displayRectangleForm).fadeIn(100);});
  });

  $('#edit-whiteboard-rectangle-cancel').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#whiteboard-rectangle').hide(100);
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(cancelRectangleForm).fadeIn(100);});
  });

  $('#edit-whiteboard-rectangle-submit').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#whiteboard-rectangle').hide(100);
    drawRectangle();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(hiddenRectangleForm).fadeIn(100);});
  });

  function drawRectangle() {
    var width = $('#edit-whiteboard-rectangle-width').val()
    var height = $('#edit-whiteboard-rectangle-height').val();
    gr.drawRectangle(pen, point, width, height);
    points = new Array();
  }

  $('#edit-whiteboard-ellipse').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#whiteboard-ellipse').show(100);
    $('#whiteboard-ellipse-container form *').css('display', 'inline');
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(displayEllipseForm).fadeIn(100);});
  });

  $('#edit-whiteboard-ellipse-cancel').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#whiteboard-ellipse').hide(100);
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(cancelEllipseForm).fadeIn(100);});
  });

  $('#edit-whiteboard-ellipse-submit').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#whiteboard-ellipse').hide(100);
    drawEllipse();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(hiddenEllipseForm).fadeIn(100);});
  });

  function drawEllipse() {
    var width = $('#edit-whiteboard-ellipse-width').val()
    var height = $('#edit-whiteboard-ellipse-height').val();
    gr.drawEllipse(pen, point, width, height);
    points = new Array();
  }

$('#edit-whiteboard-circle').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#whiteboard-circle').show(100);
    $('#whiteboard-circle-container form *').css('display', 'inline');
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(displayCircleForm).fadeIn(100);});
  });

  $('#edit-whiteboard-circle-cancel').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#whiteboard-circle').hide(100);
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(cancelCircleForm).fadeIn(100);});
  });

  $('#edit-whiteboard-circle-submit').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    $('#whiteboard-circle').hide(100);
    drawCircle();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(hiddenCircleForm).fadeIn(100);});
    points = new Array();
  });

  function drawCircle() {
    var radius = $('#edit-whiteboard-circle-radius').val();
    gr.drawCircle(pen, point, radius);
    points = new Array();
  }

  $('#edit-whiteboard-line').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    drawLine();
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(drawLineMsg).fadeIn(100);});
    points = new Array();
  });

  $('#edit-whiteboard-polyline').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    gr.drawPolyline(pen, points);
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(drawPolyline).fadeIn(100);});
    points = new Array();
  });

  $('#edit-whiteboard-polygon').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    gr.drawPolygon(pen, points);
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(drawPolygon).fadeIn(100);});
    points = new Array();
  });

  $('#edit-whiteboard-closed-curve').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    gr.drawClosedCurve(pen, points);
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(drawClosedCurve).fadeIn(100);});
    points = new Array();
  });

  $('#edit-whiteboard-bezier').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    gr.drawPolyBezier(pen, points);
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(drawBezierCurve).fadeIn(100);});
    points = new Array();
  });



  $('#edit-whiteboard-curve').click(function (e) {
    e.preventDefault();
    e.stopPropagation();
    tension = $('#tension').val();
    gr.drawCurve(pen, points, tension);
    $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(drawCurve).fadeIn(100);});
    points = new Array();
  });

  $('#edit-whiteboard-submit').click(function() {
    $.ajax({
      type: 'POST',
      url: Drupal.settings.basePath + 'whiteboard/post',
      dataType: 'json',
      data: {message:$('#whiteboard').html(),
             nodeId:Drupal.settings.whiteboard.nodeId },
      success: function(){ $('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(saved).fadeIn(100);}); },
      error: function(){$('#whiteboard-status').fadeOut(100, function() { $('#whiteboard-status').html(saveFailed).show(100);}); }
    });
  });

  $( "#whiteboard" ).mousedown(function() {
    $('#whiteboard').css('cursor', 'crosshair');
    return false;
  });

  function mouseUp() {
    mouseDown = false;
  }

  function mouseUp() {
    mouseDown = false;
  }

  //Get mouse position
  function getMouseXY(e) {
      mouseX = e.pageX - offset.left;
      mouseY = e.pageY - offset.top;

    if (system == "cartecian" ) {
      mouseX = mouseX - x;
      mouseY = y - mouseY;
      $('#mousieX').val(Math.round(mouseX));
      $('#mousieY').val(Math.round(mouseY));
    } else {
      $('#mousieX').val(Math.round(mouseX - 1));
      $('#mousieY').val(Math.round(mouseY - 1));
    }

    if (mouseDown) {
        drawPoint();
        drawLine();
    }
  }

  function mouseDownDraw() {
    mouseDown = true;
    drawPoint();
  }

  whiteboard.onmousemove = getMouseXY;
  whiteboard.onmouseup = mouseUp;
  whiteboard.onmousedown = mouseDownDraw;

  gr.setCoordinateSystem('default');
  system = 'default';
  gr.setOrigin(defaultOrigin);
  $('#whiteboard-status').fadeOut(1000, function() { $('#whiteboard-status').html(whiteboardLoaded).fadeIn(1200);});
}
}
};
}(jQuery));
