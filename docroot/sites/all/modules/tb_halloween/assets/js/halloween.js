;(function(){
	var nextFrame = (function() {
		    return window.requestAnimationFrame
				|| window.webkitRequestAnimationFrame
				|| window.mozRequestAnimationFrame
				|| window.oRequestAnimationFrame
				|| window.msRequestAnimationFrame
				|| function(callback) { return setTimeout(callback, 17); }
		})(),
		cancelFrame = (function () {
		    return window.cancelRequestAnimationFrame
				|| window.webkitCancelAnimationFrame
				|| window.webkitCancelRequestAnimationFrame
				|| window.mozCancelRequestAnimationFrame
				|| window.oCancelRequestAnimationFrame
				|| window.msCancelRequestAnimationFrame
				|| clearTimeout
		})(),

		floor = function(r) {
			return r >> 0;
		},

		random = function(min, max){
			return floor(Math.random() * (max - min) + min);
		},

		cokie = function(key, value, options) {
			if (value !== undefined) {

				options = options || {};
				
				if (value === null) {
					options.expires = -1;
				}

				return (document.cookie = [
					encodeURIComponent(key), '=', encodeURIComponent(value),
					options.expires ? '; expires=' + options.expires.toUTCString() : '', // use expires attribute, max-age is not supported by IE
					options.path    ? '; path=' + options.path : '',
					options.domain  ? '; domain=' + options.domain : '',
					options.secure  ? '; secure' : ''
				].join(''));
			}

			var cookies = document.cookie.split('; ');
			for (var i = 0, l = cookies.length; i < l; i++) {
				var parts = cookies[i].split('=');
				if (decodeURIComponent(parts.shift()) === key) {
					var cookie = decodeURIComponent(parts.join('='));
					return cookie;
				}
			}
		},

		viewport = (function()	{
			var e = window, a = 'inner';
			if (!('innerWidth' in window)){
				a = 'client';
				e = document.documentElement || document.body;
			}
			return {width : e[a + 'Width'], height: e[a + 'Height']}
		})();

	Manager = function(option){
		this.insts = [];
	};

	Manager.prototype = {
		update: function(now){
			for(var insts = this.insts, i = 0, il = insts.length; i < il; i++){
				if(insts[i] && insts[i].update){
					insts[i].update(now);
				}
			}
		},
		loop: function () {
			if (this.animating) return;

			this.animating = true;
			var self = this,
				last = new Date().getTime(),
				now = last,
				animate = function () {
					now = new Date().getTime();
					if(now - last > 15) {
						self.update(now);
						last = now;	
					} //30 fps
					
					if (self.animating) self.aniTime = nextFrame(animate);
				};
			
			animate();	
		},
		stop: function () {
			this.animating = false;
		},
		register: function(animobj){
			var existed = false;
			for (var i = 0, il = this.insts.length; i < il; i++){
				if (this.insts[i] === animobj){
					existed = true;
					break;
				}
			}

			if(!existed){
				this.insts.push(animobj);
			}
			
			if(this.insts.length){
				this.loop();
			}
		},
		unregister: function(animobj){
			for (var i = this.insts.length; i--;){
				if (this.insts[i] === animobj){
					this.insts.splice(i, 1);
				}
			}

			if(!this.insts.length){
				this.stop();
			}
		}
	};

	JAAnimManager = new Manager();

	Frame = function (options) {
		var def = {
			id: 'halloween-1',
			cls: '',							//additional class apply for the animation element
			image: 'images/pumpkin.png',		//image path
			width: 180,							//width of element, should be the same as one frame width
			height: 80,							//height of element, should be the same as one frame height
			frame: 45,							//number of frame
			fps: 45,							//fps of swing - frame per second
			type: 'random', // random, preset			//type of moving 'random' or 'preset'
			data: ['auto', 'auto', 'auto', 'auto'],		//data array, if type == random, the data should be the rectangle of region, if type == preset, please set this is an array of position [x1, y1, x2, y2, x3, y3, ......]
			duration: 3000,								//time for moving duration of moving
			delay: 2000,								//time for stay
			delaystart: 0,								//delay start - delay time before the animation start - optional 
			framestart: 0,								//frame start - the frame start - optional
			tip: 'Wish you a scary Halloween!',			//tooltip - can be HTML
			closable: true,							//show-hide close button
			action: false								//action execute when click - should be a function
		};

		for(var p in options){
			if(options.hasOwnProperty(p)){
				def[p] = options[p];
			}
		}

		if(!def.image || !def.data || !def.data.length || cokie(def.id)) {
			return;
		}

		if(def.type == 'random'){
			def.data[0] = def.data[0] == 'auto' ? 0 : def.data[0];
			def.data[1] = def.data[1] == 'auto' ? 0 : def.data[1];
			def.data[2] = def.data[2] == 'auto' ? (viewport.width - def.width) : def.data[2];
			def.data[3] = def.data[3] == 'auto' ? (viewport.height - def.height) : def.data[3];
		} else if (def.type == 'preset'){
			this.preset = [];
			this.pidx = 0;
			for(var i = 0, il = ((def.data.length >> 1) << 1); i < il; i += 2){
				this.preset.push([def.data[i], def.data[i + 1]]);
			}
		}

		this.def = def;

		var container = document.getElementById('page') || document.body,
			anim = document.createElement('div'),
			self = this;

		anim.className = 'jaanim' + ' ' + def.cls;
		anim.innerHTML = '<div class="inner">' +
							'<div class="anim">&nbsp;</div>' +
						'</div>';
		anim.style.width = def.width + 'px';
		anim.style.height = def.height + 'px';
		anim.style.backgroundImage = 'url(' + def.image + ')';
		container.appendChild(anim);
		this.anim = anim;

		if(def.tip){
			var tip = document.createElement('div');
			tip.className = 'jaatip';
			tip.innerHTML = '<p> ' + def.tip + ' <p>';

			tip.style.display = 'none';
			tip.style.opacity = '0';

			this.tip = tip;
			var tipwrap = document.createElement('div')
			tipwrap.className = 'tipwrap';
			tipwrap.appendChild(tip);
			
			if(def.closable){
				var btndel = document.createElement('a');
				btndel.innerHTML = 'delete';
				btndel.href = 'javascript:;';
				btndel.title= '';

				btndel.onclick = function(){
					anim.parentNode.removeChild(anim);
					JAAnimManager.unregister(self);
					if(def.closable == 2){
						cokie(def.id, 'hide');
					}
				};
				
				tip.appendChild(btndel);
			}
			
			anim.appendChild(tipwrap);
		}

		var mouseenter = function(){
				if(def.tip){
					clearTimeout(tip.sid);

					tip.style.display = 'block';
					tip.className = tip.className.replace(/hflip/, '').replace(/\s+/, ' ');
					if(self.anim.offsetLeft + self.anim.offsetWidth + tip.offsetWidth > viewport.width){
						tip.className = tip.className + ' hflip';
					}

					setTimeout(function(){
						tip.style.opacity = '1';
					}, 10);
				}

				self.freeze = true;
			},
			mouseleave = function(){
				if(def.tip){
					tip.sid = setTimeout(function(){
						tip.style.opacity = '0';
						tip.sid = setTimeout(function () {
							tip.style.display = 'none';
						}, 1000);
					}, 500);
				}

				self.freeze = false;
				self.reset();
			};

		anim.onmouseover = mouseenter;
		anim.onmouseout = mouseleave;

		if(Object.prototype.toString.call(def.action) == '[object Function]') {
			anim.onclick = def.action;
		}

		JAAnimManager.register(this);

		this.start();
	};

	Frame.prototype = {

		start: function () {
			this.reset();
			this.frame = this.def.framestart;
			this.duration = this.def.delaystart;
			this.x = this.xx = this.cx = random(0, viewport.width);
			this.y = this.yy = this.cy = random(0, viewport.height);
		},

		reset: function(){
			this.flast = this.last = new Date().getTime();
			this.ftime = floor(1000 / this.def.fps);
			this.phrase = 'wait';
			this.x = this.xx = this.cx;
			this.y = this.yy = this.cy;
			this.duration = this.def.delay;
		},

		update: function (now) {
			var def = this.def;
			if(!this.freeze && now > this.last + this.duration){
				this.phrase = this.phrase == 'wait' ? 'move' : 'wait';
				this.duration = this.phrase == 'wait' ? def.delay : def.duration;
				this.last = now;

				if(this.phrase == 'move'){
					var xx, yy;

					if(def.type == 'random'){
						xx = random(def.data[0], def.data[2]);
						yy = random(def.data[1], def.data[3]);

						if(xx < def.data[0]){
							xx = def.data[0];
						} else if (xx > def.data[2]){
							xx = def.data[2];
						}

						if(yy < def.data[1]){
							yy = def.data[1];
						} else if (yy > def.data[3]){
							yy = def.data[3];
						}
					} else if (def.type == 'preset'){
						this.pidx++;
						if(this.pidx >= this.preset.length){
							this.pidx = 0;
						}

						xx = this.preset[this.pidx][0];
						yy = this.preset[this.pidx][1];
					}

					this.x = this.xx;
					this.y = this.yy;

					this.xx = xx;
					this.yy = yy;
				}
			}

			if(!this.freeze && this.phrase == 'move'){
				var delta = (now - this.last) / this.duration;
				this.cx = floor((this.xx - this.x) * delta + this.x);
				this.cy = floor((this.yy - this.y) * delta + this.y);
			}

			if(this.flast + this.ftime < now){
				this.frame++;
				if(this.frame > def.frame){
					this.frame = 0;
				}

				this.flast = now;
			}
			this.render(now);
		},

		render: function (now) {
			this.anim.style.left = this.cx + 'px';
			this.anim.style.top = this.cy + 'px';
			this.anim.style.backgroundPosition = '0 -' + (this.frame * this.def.height) + 'px';
		}

	};
})();