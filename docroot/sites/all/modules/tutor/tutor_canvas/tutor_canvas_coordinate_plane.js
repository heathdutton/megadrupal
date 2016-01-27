/**
 * @file
 * Draws coordinate systems on HTML5 canvases by using settings prepared by the
 * TutorCanvas class.
 */

var canvas;
var context;
var graph;
var i;
var j;
var x;
var y;

// Loop through all the graphs prepared by the TutorCanvas class.
for (graph = 0; graph < Drupal.settings.tutorCanvas.graphData.length; graph++) {
  canvas = document.getElementById(Drupal.settings.tutorCanvas.graphData[graph].id);
  context = canvas.getContext("2d");

  // Draw the x axis
  context.lineWidth = 1;
  context.beginPath();
  context.moveTo(0, Drupal.settings.tutorCanvas.graphData[graph].origin.y);
  context.lineTo(canvas.width, Drupal.settings.tutorCanvas.graphData[graph].origin.y);
  context.stroke();

  // Draw arrow at the positive end of the axis
  context.lineWidth = .7;
  context.beginPath();
  context.moveTo(canvas.width - 5, Drupal.settings.tutorCanvas.graphData[graph].origin.y - 4);
  context.lineTo(canvas.width - 1, Drupal.settings.tutorCanvas.graphData[graph].origin.y);
  context.lineTo(canvas.width - 5, Drupal.settings.tutorCanvas.graphData[graph].origin.y + 4);
  context.stroke();

  // Draw the y axis
  context.lineWidth = 1;
  context.beginPath();
  context.moveTo(Drupal.settings.tutorCanvas.graphData[graph].origin.x, canvas.height);
  context.lineTo(Drupal.settings.tutorCanvas.graphData[graph].origin.x, 0);
  context.stroke();

  // Draw arrow at the positive end of the axis
  context.lineWidth = .7;
  context.beginPath();
  context.moveTo(Drupal.settings.tutorCanvas.graphData[graph].origin.x - 4, 5);
  context.lineTo(Drupal.settings.tutorCanvas.graphData[graph].origin.x, 1);
  context.lineTo(Drupal.settings.tutorCanvas.graphData[graph].origin.x + 4, 5);
  context.stroke();

  if (Drupal.settings.tutorCanvas.graphData[graph].drawGrid) {
    for (x = Drupal.settings.tutorCanvas.graphData[graph].origin.x; x < canvas.width; x = x + Drupal.settings.tutorCanvas.graphData[graph].grid.x) {
      context.lineWidth = .1;
      context.beginPath();
      context.moveTo(x, 0);
      context.lineTo(x, canvas.height);
      context.stroke();
    }
    for (x = Drupal.settings.tutorCanvas.graphData[graph].origin.x; x > 0; x = x - Drupal.settings.tutorCanvas.graphData[graph].grid.x) {
      context.lineWidth = .1;
      context.beginPath();
      context.moveTo(x, 0);
      context.lineTo(x, canvas.height);
      context.stroke();
    }

    for (y = Drupal.settings.tutorCanvas.graphData[graph].origin.y; y < canvas.height; y = y + Drupal.settings.tutorCanvas.graphData[graph].grid.y) {
      context.lineWidth = .1;
      context.beginPath();
      context.moveTo(0, y);
      context.lineTo(canvas.width, y);
      context.stroke();
    }
    for (y = Drupal.settings.tutorCanvas.graphData[graph].origin.y; y > 0; y = y - Drupal.settings.tutorCanvas.graphData[graph].grid.y) {
      context.lineWidth = .1;
      context.beginPath();
      context.moveTo(0, y);
      context.lineTo(canvas.width, y);
      context.stroke();
    }

    if (Drupal.settings.tutorCanvas.graphData[graph].drawScale) {
      context.textBaseline = "top";
      context.textAlign = "center";
      context.fillText(Drupal.settings.tutorCanvas.graphData[graph].grid.xOriginal, Drupal.settings.tutorCanvas.graphData[graph].origin.x + Drupal.settings.tutorCanvas.graphData[graph].grid.x, Drupal.settings.tutorCanvas.graphData[graph].origin.y + 1);
      context.textAlign = "right";
      context.textBaseline = "middle";
      context.fillText(Drupal.settings.tutorCanvas.graphData[graph].grid.yOriginal, Drupal.settings.tutorCanvas.graphData[graph].origin.x - 1, Drupal.settings.tutorCanvas.graphData[graph].origin.y - Drupal.settings.tutorCanvas.graphData[graph].grid.y);
    }
  }

  for (i = 0; i < Drupal.settings.tutorCanvas.graphData[graph].dataSets.length; i++) {
    // Draw individual points, if that is the type for the data set.
    if (Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].type == 'points') {
      for (j = 0; j < Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].x.length; j++) {
        context.beginPath();
        context.arc(Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].x[j], Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].y[j], 2, 0 , 2 * Math.PI, false);
        context.strokeStyle = Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].color;
        context.lineWidth = 1;
        context.stroke();
      }

      // Add labels with coordinates, if option for this is set.
      if (Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].drawCoordinates) {
        for (j = 0; j < Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].x.length; j++) {
          context.textBaseline = "top";
          context.textAlign = "left";
          context.fillText("(" + Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].xOriginal[j] + ", " + Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].yOriginal[j] + ")", Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].x[j] + 1, Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].y[j] + 1);
        }
      }
    }

    // Draw connected lines, if this is the type for the data set.
    if (Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].type == 'curve') {
      context.beginPath();
      context.lineWidth = 1.5;
      context.strokeStyle = Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].color;
      context.moveTo(Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].x[0], Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].y[0]);
      for (j = 0; j < Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].x.length; j++) {
        context.lineTo(Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].x[j], Drupal.settings.tutorCanvas.graphData[graph].dataSets[i].y[j]);
      }
      context.stroke();
    }
  }
}
