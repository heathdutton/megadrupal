(function ($) {
  Drupal.behaviors.tic_tac_toe = {
    attach: function (context) {

      var options = {
        size : Drupal.settings.tic_tac_toe.tic_tac_toe_board_size_block,
        padding : 8
      };
      var ttt = new TicTacToe('tic-tac-toe-board', options);
      if (Const.TURN == Const.COM) {
        ttt.post();
      }
      else{
        $('#tic-tac-toe-debug').html('It is your turn. Please start the game!');
      }
    }
  };
  var TicTacToe = function(paperId, options)
  {
    var defaults = {
      size : 500,
      background : '#EEEF95',
      cell_fill : '45-#C0C0C0-#666',
      cell_stroke : '#C2C2C2',
      stroke_width : 6,
      padding : 10
    };

    options = options || {};
    this.properties = $.extend(defaults, options);
    this.isPlaying = true;
    $('#' + paperId).css({
      'width' : this.properties.size
    });
    this.canvas = Raphael(document.getElementById(paperId), this.properties.size, this.properties.size);
    this.makeBoard();
  };

  /**
  * Creates the TicTacToe board
  */
  TicTacToe.prototype.makeBoard = function()
  {
    this.canvas.clear();
    this.canvas.rect(0, 0, this.properties.size, this.properties.size).attr('fill', this.properties.background);
    var cellWidth = Math.round(this.properties.size / 3);
    for (i = 0; i < 3; i++) {
      for (j = 0; j < 3; j++) {
        var x = i * cellWidth + this.properties.padding;
        var y = j * cellWidth + this.properties.padding;
        var num = i * 3 + j;
        var cell = this.canvas.rect(y, x, cellWidth - this.properties.padding * 2, cellWidth - this.properties.padding * 2, 10)
        .attr({
          'stroke' : this.properties.cell_stroke,
          'fill' : this.properties.cell_fill
        });

        cell.node.setAttribute('id', 'cell' + num);
        var _this = this;
        cell.node.onclick = function(e){
          if($.browser.mozilla == true){
            var target = e.target;
          }else{
            var target =	window.event.srcElement;
          }
          _this.clickHandler(target);
        };
      }
    }
  };

  TicTacToe.prototype.clickHandler = function(target)
  {
    if (!this.isPlaying) {
      return;
    }
    if (Const.TURN == Const.COM) {
      return;
    }

    var cellIndex = target.raphael.id - 1;
    this.o(target);
    Position.set(cellIndex, Const.HUMAN);
    Const.TURN = Const.COM;
    this.post();
  };

  TicTacToe.prototype.x = function(target)
  {
    var width = parseInt(target.raphael.attrs.width);
    var height = parseInt(target.raphael.attrs.height);
    var startX = parseInt(target.raphael.attrs.x) + (2 * this.properties.padding);
    var startY = parseInt(target.raphael.attrs.y) + (2 * this.properties.padding);
    var endX = startX + width - (4 * this.properties.padding);
    var endY = startY + height - (4 * this.properties.padding);
    this.canvas.path('M ' + startX + ' ' + startY + 'L' + endX + ' ' + endY).attr('stroke-width', this.properties.stroke_width);
    this.canvas.path('M ' + endX + ' ' + startY + 'L' + startX + ' ' + endY).attr('stroke-width', this.properties.stroke_width);
  };

  TicTacToe.prototype.o = function(target)
  {
    var x = parseInt(target.raphael.attrs.x);
    var y = parseInt(target.raphael.attrs.y);
    var centerX = parseInt(target.raphael.attrs.width) / 2 + x;
    var centerY = parseInt(target.raphael.attrs.height) / 2 + y;
    this.canvas.circle(centerX, centerY, parseInt(target.raphael.attrs.width) / 3).attr('stroke-width', this.properties.stroke_width);
  };

  TicTacToe.prototype.post = function()
  {
    var _this = this;
    var param = 'board=' + Position.get();
    $.ajax({
      url: '?q=tic-tac-toe/game-action',
      type: "POST",
      cache: false,
      dataType: "json",
      data: param,
      beforeSend: function(){$('#tic-tac-toe-debug').html('Thinking...');},
      complete: function(){$('#tic-tac-toe-debug').html('Done');},
      success: function(data) {
        for(var i in data.board){
          if (data.board[i] != Position.get(i)) {
            _this.x(document.getElementById('cell' + i));
            break;
          }
        }
        Position.update(data.board);
        Const.TURN = Const.HUMAN;
        if(data.status != null){
          switch (data.status) {
            case Const.COM:
              _this.isPlaying = false;
              if(confirm('You Lost. Do you want to play again?')) {
                $('#tic-tac-toe-debug').html('It is your turn. Please start the game!');
                $('#tic-tac-toe-board circle').remove();
                $('#tic-tac-toe-board path').remove();
                var tempBoard = [0, 0, 0, 0, 0, 0, 0, 0, 0];
                _this.isPlaying = true;
                Const.TURN == Const.HUMAN;
                Position.update(tempBoard);
              }
              break;

            case Const.HUMAN:
              _this.isPlaying = false;
              if(confirm('You Won. Do you want to play again?')) {
                $('#tic-tac-toe-debug').html('It is your turn. Please start the game!');
                $('#tic-tac-toe-board circle').remove();
                $('#tic-tac-toe-board path').remove();
                var tempBoard = [0, 0, 0, 0, 0, 0, 0, 0, 0];
                _this.isPlaying = true;
                Const.TURN == Const.HUMAN;
                Position.update(tempBoard);
              }
              break;

            case (0):
              _this.isPlaying = false;
            if(confirm('The Game is Drawn. Do you want to play again?')) {
              $('#tic-tac-toe-debug').html('It is your turn. Please start the game!');
              $('#tic-tac-toe-board circle').remove();
              $('#tic-tac-toe-board path').remove();
              var tempBoard = [0, 0, 0, 0, 0, 0, 0, 0, 0];
              _this.isPlaying = true;
              Const.TURN == Const.HUMAN;
              Position.update(tempBoard);
            }
              break;

            default:
              alert(Drupal.t('Unknown status'));
              break;
          }
        }
      }
    });
  };

  var Position = function()
  {
    var board = [0, 0, 0, 0, 0, 0, 0, 0, 0];

    return {
      get : function(index){
        if(index == undefined){
          return board;
        }
        return board[index];
      },

      set : function(index, value){
        board[index] = value;
      },

      update : function(boardObj){
        for(var i in boardObj){
          this.set(i, boardObj[i]);
        }
      }
    };
  }();

  var Const = {
    HUMAN : 1,
    BLANK : 0,
    COM : -1,

    TURN : this.HUMAN
  };

})(jQuery);
