﻿package {	import flash.events.Event;	import flash.display.MovieClip;	import flash.display.Loader;	import flash.display.StageAlign;	import flash.display.StageScaleMode;	import flash.media.Sound;	import flash.net.URLRequest;	import flash.net.URLVariables;	import flash.net.URLLoader;	import flash.net.URLRequestMethod;	import flash.net.URLLoaderDataFormat;	import flash.events.TimerEvent;	import flash.utils.Timer;	import flash.external.ExternalInterface;	import flash.errors.IOError;	import flash.events.IOErrorEvent;	import fl.transitions.*;	import fl.transitions.easing.*;	public class FlagMovie extends MovieClip {				// tracking code		private static const CURLYPAGE_TRACKING_CODE:String = "LiPXZui3RT2i9rg2";		// unique curlypage identifier		private var cpid:uint;		// in transition for peel add. Values: Blinds, Fade, Fly, Iris, Photo, Rotate, Squeeze, Wipe, PixelDissolve, Zoom.		private var inTransition:String;		// transition duration parameter		private var transitionDuration:uint;				// style of flag		private var flagStyle:String;		// size of design flag and peel		private const FLAGDESIGNSIZE:uint = 100;		// flag width and height. Suggested values 10-200		private var flagWidth:uint;		private var flagHeight:uint;				// peel position on page. Values: topleft || topright || bottomleft || bottomright		private var peelPosition:String;		// ad image on close peel		private var smallURL:String;		// mirror the image on peel. Values: true || false		private var mirror:Boolean;		// color of peel. Values: golden || silver || custom		private var peelColor:String;		// color of peel style. Value: flat || gradient		private var peelColorStyle:String;		// red value of custom color. Values 0-255		private var redValue:uint;		// green value of custom color. Values 0-255		private var greenValue:uint;		// blue value of custom color. Values 0-255		private var blueValue:uint;		// sound to play when peel is loaded		private var loadSoundURL:String;		// open with click. Values: 0/1		private var open_onclick:uint;		// speed of flag movement. Values: 1-9		private var flagSpeed:uint;		// enable tracking. Values: true || false		private var tracking:Boolean;		// test variables for media load		private var small_opened:Boolean = false;		private var load_sound_opened:Boolean = false;		private var small_loaded:Boolean = false;		private var load_sound_loaded:Boolean = false;		private var peel_loaded:Boolean = false;		// loader for small image and mirror image		private var small_ldr:Loader;		// loader for load sound		private var loadSound:Sound = null;				// delay seconds, curlypage start delayed		private var startDelay:uint;		private var timeSlot:uint;				private var theflag:MovieClip;		private var delayTimer:Timer;		private var timeSlotTimer:Timer;		// transition manager		private var tm:TransitionManager;		/*		 * FlagMovie Constructor		 */		public function FlagMovie () {			stage.align = StageAlign.TOP_LEFT;			stage.scaleMode = StageScaleMode.NO_SCALE;			ExternalInterface.addCallback("startTimeSlot", startTimeSlot);			ExternalInterface.addCallback("peelLoaded", peelLoaded);			loadParams();			preload();			// listen for browser resizing			var e = new Event("null");			stage.addEventListener(Event.RESIZE, onStageResize);			onStageResize(e);		}		/**		 * Resize the movie taking into account browser zoom (stage size)		 */		private function onStageResize(e:Event):void {    		var widthScale:Number = stage.stageWidth / this.flagWidth;			var heightScale:Number = stage.stageHeight / this.flagHeight;    		scaleX = widthScale;    		scaleY = heightScale;		}		/*		 * Load Flag Movie Parameters		 */		private function loadParams () : void {						cpid = this.loaderInfo.parameters.cpid || 0;			inTransition = this.loaderInfo.parameters.inTransition || "Squeeze";			transitionDuration = this.loaderInfo.parameters.transitionDuration || 4;						flagStyle = this.loaderInfo.parameters.flagStyle || "style1";			flagWidth = this.loaderInfo.parameters.flagWidth || FLAGDESIGNSIZE;			flagHeight = this.loaderInfo.parameters.flagHeight || FLAGDESIGNSIZE;			peelPosition = this.loaderInfo.parameters.peelPosition || "topright";			smallURL = this.loaderInfo.parameters.smallURL || "small.jpg"; 			mirror = Boolean(uint(this.loaderInfo.parameters.mirror)) || false;			peelColor = this.loaderInfo.parameters.peelColor || "custom";			peelColorStyle = this.loaderInfo.parameters.peelColorStyle || "gradient";			redValue = this.loaderInfo.parameters.redValue || 0;			greenValue = this.loaderInfo.parameters.greenValue || 0;			blueValue = this.loaderInfo.parameters.blueValue || 0;			loadSoundURL = this.loaderInfo.parameters.loadSoundURL || "";			open_onclick = this.loaderInfo.parameters.open_onclick || 0;			flagSpeed = this.loaderInfo.parameters.flagSpeed || 4;			startDelay = this.loaderInfo.parameters.startDelay * 1000 || 0;			timeSlot = this.loaderInfo.parameters.timeSlot * 1000 || 0;			tracking = this.loaderInfo.parameters.tracking || false;		}				/*		 * start the Movie		 */		private function startMovie () : void {			if (startDelay > 0) {				delayTimer = new Timer(startDelay, 1);				delayTimer.addEventListener(TimerEvent.TIMER_COMPLETE, startFlag);				delayTimer.start();			}			else {				addFlag();			}		}				private function startFlag(e:TimerEvent) : void {			addFlag();		}				/*		 * add Flag		 */		private function addFlag() : void {						if (flagStyle == "style1") {				theflag = new Style1Flag(cpid,										 peelPosition,										 mirror,										 peelColor,										 peelColorStyle,										 redValue,										 greenValue,										 blueValue,										 loadSound,										 open_onclick,										 flagSpeed,										 small_ldr,										 flagWidth,										 flagHeight);			} else if (flagStyle == "style2") {				theflag = new Style2Flag(cpid,										 peelPosition,										 mirror,										 peelColor,										 peelColorStyle,										 redValue,										 greenValue,										 blueValue,										 loadSound,										 open_onclick,										 flagSpeed,										 small_ldr,										 flagWidth,										 flagHeight);			} else if (flagStyle == "style3") {				theflag = new Style3Flag(cpid,										 peelPosition,										 mirror,										 peelColor,										 peelColorStyle,										 redValue,										 greenValue,										 blueValue,										 loadSound,										 open_onclick,										 flagSpeed,										 small_ldr,										 flagWidth,										 flagHeight);			}			ExternalInterface.call("curlypage_start", cpid, peelPosition);			// track close view, flag has been shown.			if (tracking) {				curlypageView(this.cpid);			}						if (timeSlot > 0) {				startTimeSlot();			}			addChild(theflag);			if (inTransition != "none") {				doTransition();			}		}				private function startTimeSlot() : void {			timeSlotTimer = new Timer(timeSlot, 1);			timeSlotTimer.addEventListener(TimerEvent.TIMER_COMPLETE, finishCurlypage);			timeSlotTimer.start();		}		private function finishCurlypage(e:TimerEvent) : void {			ExternalInterface.call("curlypage_next", cpid, peelPosition);		}		/*		 * Test if all media is loaded 		 */		private function checkAllLoaded () : void {			if (!peel_loaded) {				return;			}			if (!small_loaded) {				return;			}						if (!load_sound_loaded) {				if (loadSoundURL != "") {					return;				}			}				startMovie();		}		/*		 * Test if all media has started loading 		 */		private function checkAllOpened () : void {			if (!small_opened) {				return;			}						if (!load_sound_opened) {				if (loadSoundURL != "") {					return;				}			}				ExternalInterface.call("curlypage_hide_flag", cpid, peelPosition);		}		/*		 * small image start load event handler		 */		private function smallImageOpened (e:Event) : void {			small_opened = true;			checkAllOpened();		}		/*		 * small image load event handler		 */		private function smallImageLoaded (e:Event) : void {			small_loaded = true;						// set FPS			if (small_ldr.contentLoaderInfo.contentType == "application/x-shockwave-flash") {				stage.frameRate = small_ldr.contentLoaderInfo.frameRate;			}			checkAllLoaded();		}		/*		 * load sound start load event handler		 */		 private function loadSoundOpened(e:Event) : void {			 load_sound_opened = true;			 checkAllOpened();		 }		/* 		 * load sound load event handler 		 */		private function loadSoundLoaded (e:Event) : void {			load_sound_loaded = true;			checkAllLoaded();		}		/*		 * peel media has been loaded		 */		private function peelLoaded() {			peel_loaded = true;			checkAllLoaded();		}		/*		 * Preload images & sounds		 */		private function preload() {						small_ldr = new Loader();			small_ldr.contentLoaderInfo.addEventListener(Event.OPEN, smallImageOpened);			small_ldr.contentLoaderInfo.addEventListener(Event.COMPLETE, smallImageLoaded);			small_ldr.contentLoaderInfo.addEventListener(IOErrorEvent.IO_ERROR, ioErrorHandler);			small_ldr.load(new URLRequest(smallURL));			if (loadSoundURL != "") {				loadSound = new Sound();				loadSound.addEventListener(Event.OPEN, loadSoundOpened);				loadSound.addEventListener(Event.COMPLETE, loadSoundLoaded);				loadSound.addEventListener(IOErrorEvent.IO_ERROR, ioErrorHandler);				loadSound.load(new URLRequest(loadSoundURL));			}		}		/*		 * Error handler		 */		private function ioErrorHandler (e:IOErrorEvent) : void {			// try to close all loads in progress			try {				small_ldr.close();				loadSound.close();			}			catch (e:IOError) {				trace("Could not close some stream");			}			finally {				// set objects to null				small_ldr = null;				loadSound = null;				ExternalInterface.call("curlypage_error_destroy", cpid, peelPosition);							trace("ioErrorHandler: " + e);			}		}		/*		 * Track views function		 */		private function curlypageView(cpid:uint) : void {			var loader:URLLoader;			var variables:URLVariables;			var req:URLRequest;						variables = new URLVariables();			variables.status = "close";			variables.source = CURLYPAGE_TRACKING_CODE;			req = new URLRequest("/curlypage/" + cpid + "/view");			req.data = variables;			req.method = URLRequestMethod.POST;						loader = new URLLoader();			loader.dataFormat = URLLoaderDataFormat.TEXT;			loader.addEventListener(Event.COMPLETE, completedView);			loader.addEventListener(IOErrorEvent.IO_ERROR, ioErrorHandlerView);			try {				loader.load(req);			} catch (error:Error) {                trace("Unable to load requested document.");            }		}				/*		 * Complete view handler		 */		private function completedView (e:Event) : void {		    // we don't need to do anything with returned data            var loader:URLLoader;            loader = URLLoader(e.target);			trace (loader.data);		}				/*		 * Error handler		 */		private function ioErrorHandlerView (e:IOErrorEvent) : void {			// we are not going to do anything even in case of an error			trace("ioErrorHandler: " + e);		}		/*		 * in transition if it is enabled		 */		private function doTransition () : void {			tm = new TransitionManager(this);					switch (inTransition) {				case "Blinds":					tm.startTransition({type:Blinds, direction:Transition.IN, duration:transitionDuration, easing:Strong.easeOut, numStrips:40, dimension:0});					break;				case "Fade":					tm.startTransition({type:Fade, direction:Transition.IN, duration:transitionDuration, easing:Strong.easeInOut});					break;				case "Fly":					var flyStartPoint:uint;					switch (peelPosition) {						case "topleft":							flyStartPoint = 9;							break;						case "topright":							flyStartPoint = 7;							break;						case "bottomleft":							flyStartPoint = 3;							break;						case "bottomright":							flyStartPoint = 1;							break;					}					tm.startTransition({type:Fly, direction:Transition.IN, duration:transitionDuration, easing:Elastic.easeOut, startPoint:flyStartPoint});					break;				case "Iris":					var irisStartPoint:uint;					switch (peelPosition) {						case "topleft":							irisStartPoint = 1;							break;						case "topright":							irisStartPoint = 3;							break;						case "bottomleft":							irisStartPoint = 7;							break;						case "bottomright":							irisStartPoint = 9;							break;					}					tm.startTransition({type:Iris, direction:Transition.IN, duration:transitionDuration, easing:Elastic.easeOut, startPoint:irisStartPoint, shape:Iris.CIRCLE});					break;				case "Photo":					tm.startTransition({type:Photo, direction:Transition.IN, duration:transitionDuration, easing:Strong.easeOut});					break;				case "Rotate":					var rotationCCW:Boolean;					if (peelPosition == "topright" || peelPosition == "bottomleft") {						rotationCCW = true;					} else {						rotationCCW = false;					}					tm.startTransition({type:Zoom, direction:Transition.IN, duration:transitionDuration, easing:None.easeInOut});					tm.startTransition({type:Rotate, direction:Transition.IN, duration:transitionDuration, easing:Back.easeInOut, ccw:rotationCCW, degrees:360});					break;				case "Squeeze":					tm.startTransition({type:Squeeze, direction:Transition.IN, duration:transitionDuration, easing:Elastic.easeOut, dimension:0});					break;				case "Wipe":					var wipeStartPoint:uint;					switch (peelPosition) {						case "topleft":							wipeStartPoint = 1;							break;						case "topright":							wipeStartPoint = 3;							break;						case "bottomleft":							wipeStartPoint = 7;							break;						case "bottomright":							wipeStartPoint = 9;							break;					}					tm.startTransition({type:Wipe, direction:Transition.IN, duration:transitionDuration, easing:Strong.easeOut, startPoint:wipeStartPoint});					break;				case "PixelDissolve":					tm.startTransition({type:PixelDissolve, direction:Transition.IN, duration:transitionDuration, easing:Strong.easeOut, xSections:40, ySections:40});					break;				case "Zoom":					tm.startTransition({type:Zoom, direction:Transition.IN, duration:transitionDuration, easing:Strong.easeOut});					break;				default:			}		}	}}