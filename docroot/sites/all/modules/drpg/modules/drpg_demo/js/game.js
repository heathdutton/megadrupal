(function ($) {

  Drupal.behaviors.DrpgDemoGame = {
    attach: function (context, settings) {

      function DrpgGame() {
        this.serverUrl = Drupal.settings.drpgServerUrl;
        this.imagePath = Drupal.settings.drpgImagePath;

        // Path used to get player avatar data.
        this.avatarDataPath = 'drpg/data/avatar/';
        // Path used to get game items data.
        this.itemsDataPath = 'drpg/data/items/';
        // Path used to get game room data.
        this.roomDataPath = 'drpg/data/room/';

        // The width of an individual game map tile.
        this.tileWidth = 32;
        // The height of an individual game map tile.
        this.tileHeight = 32;

        // The HTML5 canvas object.
        this.canvas = null;
        // The HTML5 canvas context object.
        this.context = null;

        // The game console, used to output text.
        this.console = null;

        this.roomMaps = {
          1: {

            /**
             * Each element in this array represents a line of map tiles.
             *
             * Each array contains two numeric values indicating the
             * location of a tile in the map sprite sheet. Note that the tile
             * locations defined here are not pixel positions, but locations
             * relative to other tiles.
             * [1,2] would define a tile one tile from the left and two tiles
             * from the top of the sprite sheet
             *
             * The map is drawn from top to bottom, left to right, in the order
             * defined in this array.
             */

            map: [
              [[12, 0], [12, 0], [12, 0], [12, 0], [12, 0], [12, 0], [12, 0], [12, 0], [12, 0], [12, 0], [12, 0]],
              [[12, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [12, 0]],
              [[12, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [12, 0]],
              [[12, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [12, 0]],
              [[12, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [12, 0]],
              [[12, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [12, 0]],
              [[12, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [13, 0], [12, 0]],
              [[12, 0], [12, 0], [12, 0], [12, 0], [12, 0], [12, 0], [12, 0], [12, 0], [12, 0], [12, 0], [12, 0]]
            ],

            /**
             * Item container nodes define coordinates on the map where item
             * containers may be spawned. These nodes will be populated with
             * item containers when the room data is loaded from the server.
             *
             * The number of item containers in a room will not exceed the
             * number of item container nodes, even if there are more item
             * containers in the data retrieved from the server.
             */

            itemContainerNodes: [
              {
                x: 2,
                y: 4
              },
              {
                x: 7,
                y: 2
              },
              {
                x: 5,
                y: 1
              }
            ],
            doorNodes: []
          }
        };

        // The player object. Populated with player data retrieved from the server.
        this.player = {};

        // The ID of the game room the player currently exists in.
        this.room_id = 1;

        // The current room object. Populated with room data retrieved from the server.
        this.room = {};

        // The current room's item containers.
        this.itemContainers = [];

        // The Image object containing the map tile sprite sheet.
        this.mapImageObj = null;

        // The Image object containing the player image.
        this.playerImageObj = null;
        // The image data appearing behind the player image.
        // Used to redraw the background when the player moves.
        this.playerBackgroundImageData = null;

        // Array of Image objects used to display items in-game.
        this.itemImageCache = [];

        // Array of Image objects used to display game item UI.
        this.itemUIImageCache = [];

        // The pixel dimensions of the map.
        this.mapDimensions = {
          x: 0,
          y: 0
        };

        this.playerPosition = {
          x: 5,
          y: 4
        };
      }

      DrpgGame.prototype = {
        /**
         * Sets up the initial game client click handlers.
         *
         * @returns {undefined}
         */
        init: function () {
          this.console = $("#drpg-game-console");

          // Cache item chest image.
          itemImageObj = new Image();
          itemImageObj.src = this.serverUrl + this.imagePath + 'items/chest.png';
          // Give item chest item ID 0. ID unused by regular game items.
          this.itemImageCache[0] = itemImageObj;

          this.loadItemsData();
          this.loadAvatarData();
          this.loadRoomData();
        },

        /**
         * Adds a message to the game's console.
         *
         * @param {string} message The message to add.
         */
        addConsoleMessage: function (message) {
          this.console.append('<p>' + message + '</p>');
          this.console.scrollTop(this.console[0].scrollHeight);
        },

        /**
         * Requests the current user's avatar data from the server.
         *
         * @returns {undefined}
         */
        loadAvatarData: function () {
          this.addConsoleMessage('Loading avatar data from ' + this.avatarDataPath);

          $.ajax({
            context: this,
            type: 'get',
            url: this.serverUrl + this.avatarDataPath,
            success: function(response) {
              this.handleAvatarDataLoad(response);
            },
          });
        },

        /**
         * Handles the result of an avatar data request.
         *
         * @param {Object} response Server response to the avatar data request.
         * @returns {undefined}
         */
        handleAvatarDataLoad: function (response) {
          if (response.success) {
            this.addConsoleMessage('Avatar data loaded.');
            this.player.avatar = response.avatar;
          }
          else {
            this.addConsoleMessage('Failed to load avatar data.');
          }
        },

        /**
         * Requests the game items data from the server.
         *
         * @returns {undefined}
         */
        loadItemsData: function () {
          this.addConsoleMessage('Loading items data from ' + this.itemsDataPath);

          $.ajax({
            context: this,
            type: 'get',
            url: this.serverUrl + this.itemsDataPath,
            success: function(response) {
              this.handleItemsDataLoad(response);
            },
          });
        },

        /**
         * Handles the result of an items data request.
         *
         * @param {Object} response Server response to the items data request.
         * @returns {undefined}
         */
        handleItemsDataLoad: function (response) {
          if (response.success) {
            this.addConsoleMessage('Items data loaded.');

            var item = null;
            for (var item_id in response.items) {
              item = response.items[item_id];

              if (item.ui_asset_path.length > 0) {
                itemImageObj = new Image();
                itemImageObj.src = this.serverUrl + this.imagePath + item.ui_asset_path;

                // Cache item UI image.
                this.itemUIImageCache[item.item_id] = itemImageObj;
              }
            }
          }
          else {
            this.addConsoleMessage('Failed to load items data.');
          }
        },

        /**
         * Requests the current room data from the server.
         *
         * @returns {undefined}
         */
        loadRoomData: function () {
          this.addConsoleMessage('Loading room data from ' + this.roomDataPath);

          $.ajax({
            context: this,
            type: 'get',
            url: this.serverUrl + this.roomDataPath + this.room_id,
            success: function(response) {
              this.handleRoomDataLoad(response);
            },
          });
        },

        /**
         * Handles the result of a room data request.
         *
         * @param {Object} response Server response to the room data request.
         * @returns {undefined}
         */
        handleRoomDataLoad: function (response) {
          if (response.success) {
            this.addConsoleMessage('Room data loaded.');

            this.room = response.room;

            this.createMap();
          }
          else {
            this.addConsoleMessage('Failed to load room data.');
          }
        },

        /**
         * Starts the game map rendering process by loading the map tiles
         *  sprite sheet.
         *
         * @returns {undefined}
         */
        createMap: function (room) {
          this.addConsoleMessage('Loading map spritesheet.');

          this.mapImageObj = new Image();
          this.mapImageObj.onload = $.proxy(this.drawMap, this);
          this.mapImageObj.src = this.serverUrl + this.imagePath + 'tiles-spritesheet.png';
        },

        /**
         * Renders the game map once the map tiles sprite sheet has been loaded.
         *
         * @returns {undefined}
         */
        drawMap: function () {
          this.addConsoleMessage('Map spritesheet loaded.');
          this.addConsoleMessage('Drawing map.');

          var sourceX = 0;
          var sourceY = 0;

          var destinationX = 0;
          var destinationY = 0;

          var map = this.roomMaps[this.room_id].map;

          // Draw each row of tiles in the map array.
          for (var i = 0; i < map.length; i++) {
            // For each row, draw each column of tiles.
            for (var j = 0; j < map[i].length; j++) {
              // The location of the tile on the sprite sheet's X axis multiplied by the tile width
              // gives the pixel position of this map tile on the X axis
              sourceX = (map[i][j][0] * this.tileWidth);

              // The location of the tile on the sprite sheet's Y axis multiplied by the tile height
              // gives the pixel position of this map tile on the Y axis
              sourceY = (map[i][j][1] * this.tileHeight);

              this.context.drawImage(
                this.mapImageObj, // The sprite sheet image object.
                sourceX, // The pixel position of the map tile on the sprite sheet X axis.
                sourceY, // The pixel position of the map tile on the sprite sheet Y axis.
                this.tileWidth, // The map tile width.
                this.tileHeight, // The map tile height.
                destinationX, // The X axis pixel position the map tile should be drawn onto the canvas at.
                destinationY, // The Y axis pixel position the map tile should be drawn onto the canvas at.
                this.tileWidth, // The map tile width.
                this.tileHeight // The map tile height.
              );

              destinationX += this.tileWidth;

              if (destinationX > this.mapDimensions.x) {
                this.mapDimensions.x = (destinationX + this.tileWidth);
              }
            }

            destinationX = 0;
            destinationY += this.tileHeight;

            if (destinationY > this.mapDimensions.y) {
              this.mapDimensions.y = destinationY;
            }
          }

          this.addConsoleMessage('Map drawn.');

          this.createPlayer();
          this.drawMapItems();
        },

        createPlayer: function () {
          this.playerImageObj = new Image();
          this.playerImageObj.onload = $.proxy(this.drawPlayer, this);
          this.playerImageObj.src = this.serverUrl + this.imagePath + 'player/avatar.png';
        },

        drawPlayer: function () {
          var startPos = this.getPixelPositionFromCoords(this.playerPosition);

          this.playerBackgroundImageData = this.context.getImageData(startPos.x, startPos.y, this.tileWidth, this.tileHeight);
          this.drawMapItemImage(this.playerImageObj, this.playerPosition);
        },

        /**
         * Draws the room's items on the map.
         *
         * @returns {undefined}
         */
        drawMapItems: function () {
          this.addConsoleMessage('Drawing map items.');

          var mapItem = null;

          // Get item containers from room data.
          var itemContainerNodes = this.roomMaps[this.room_id].itemContainerNodes;
          var itemContainerNode = null;
          var itemContainersDrawn = 0;

          for (var i = 0; i < itemContainerNodes.length; i++) {
            if (i >= this.room.item_containers.length) {
              continue;
            }

            itemContainerNode = itemContainerNodes[i];

            if (this.itemContainers[itemContainerNode.x] === undefined) {
              this.itemContainers[itemContainerNode.x] = [];
            }

            if (this.itemContainers[itemContainerNode.x][itemContainerNode.y] === undefined) {
              this.itemContainers[itemContainerNode.x][itemContainerNode.y] = [];
            }

            this.itemContainers[itemContainerNode.x][itemContainerNode.y] = this.room.item_containers[i];

            this.drawMapItemImage(this.itemImageCache[0], {x: itemContainerNode.x, y: itemContainerNode.y});

            itemContainersDrawn++;
          }

          this.addConsoleMessage('Drew ' + itemContainersDrawn + ' item containers.');
        },

        /**
         * Draws an image onto a game map tile at the given coordinates.
         *
         * @param {Image} imageObj The image object.
         * @param {Object} tileCoord The map tile coordinates.
         * @returns {undefined}
         */
        drawMapItemImage: function (imageObj, tileCoord) {
          this.context.drawImage(imageObj, (tileCoord.x * this.tileWidth), (tileCoord.y * this.tileHeight));
        },

        /**
         * Sets the canvas object for use by the game.
         *
         * @param {type} canvas
         * @returns {undefined}
         */
        setCanvas: function (canvas) {
          this.canvas = canvas;
          this.context = canvas.getContext('2d');

          canvas.addEventListener('keydown', $.proxy(this.handleKeyDown, this), false);
        },

        /**
         * Handles a key down event.
         *
         * @param {Object} ev The key down event.
         * @returns {undefined}
         */
        handleKeyDown: function (ev) {
          if ((ev.keyCode == 65) || (ev.keyCode == 68) || (ev.keyCode == 83) || (ev.keyCode == 87)) {
            var startPos = this.getPixelPositionFromCoords(this.playerPosition);
            this.context.putImageData(this.playerBackgroundImageData, startPos.x, startPos.y);
          }

          // W (up)
          if (ev.keyCode == 87) {
            this.playerPosition.y--;
            this.drawPlayer();
          }
          // A (left)
          else if (ev.keyCode == 65) {
            this.playerPosition.x--;
            this.drawPlayer();
          }
          // S (down)
          else if (ev.keyCode == 68) {
            this.playerPosition.x++;
            this.drawPlayer();
          }
          // D (right)
          else if (ev.keyCode == 83) {
            this.playerPosition.y++;
            this.drawPlayer();
          }
          // E (interact)
          else if (ev.keyCode == 69) {
            this.handleItemContainerInteraction(this.playerPosition);
          }
        },

        handleItemContainerInteraction: function (coords) {
          if (this.itemContainers[coords.x] !== undefined
            && this.itemContainers[coords.x][coords.y] !== undefined) {
            this.addConsoleMessage('Opened item container.');

            var itemContainer = this.itemContainers[coords.x][coords.y];

            if (itemContainer.items.length > 0) {
              for (var i = 0; i < itemContainer.items.length; i++) {
                this.addConsoleMessage('Found ' + itemContainer.items[i].label);
              }
            }
          }
        },

        getPixelPositionFromCoords: function (coords) {
          var position = {
            x: (coords.x * this.tileWidth),
            y: (coords.y * this.tileHeight)
          };

          return position;
        }
      };

      $(document).ready(function () {
        var game = new DrpgGame();
        game.init();

        // Pass canvas as plain DOM element, not jQuery object.
        // Only using it to get the canvas context.
        game.setCanvas(document.getElementById('drpg-game-canvas'));

        // Set up canvas focus handlers.
        var canvas = $('#drpg-game-canvas');
        var focusLink = $('#drpg-game-focus');

        focusLink.click(function () {
          $('#drpg-game-canvas').focus();
        });

        // When the game canvas has focus, hide focus link.
        canvas.focusin(function () {
          focusLink.hide();
        });

        // When game canvas loses focus, show the focus link.
        canvas.focusout(function (e) {
          focusLink.show();
        });

        canvas.focus();
      });
    }
  };

})(jQuery);
