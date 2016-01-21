(function (window, document) {
var menu = document.getElementById('menu'),
  WINDOW_CHANGE_EVENT = ('onorientationchange' in window) ? 'orientationchange':'resize';

function toggleHorizontal() {
  [].forEach.call(
    document.getElementById('menu').querySelectorAll('.menu-transform'),
    function(el){
      el.classList.toggle('pure-menu-horizontal');
    }
  );
};

function toggleMenu() {
  toggleHorizontal();
  menu.classList.toggle('open');
  document.getElementById('toggles').classList.toggle('x');
};

function closeMenu() {
  if (menu.classList.contains('open')) {
    toggleMenu();
  }
}

document.getElementById('toggles').addEventListener('click', function (e) {
  toggleMenu();
});

window.addEventListener(WINDOW_CHANGE_EVENT, closeMenu);
})(this, this.document);



/* fix for androids to work with menu nodes that have links */
(function($){
	if(/Android|webOS|BlackBerry|IEMobile|Opera Mini/i.test(window.navigator.userAgent)){
		$('.pure-menu-has-children > a')
			.attr('data-click', 'false').click(function(){
				if($(this).attr('data-click')=='false'){
					$(this).attr('data-click', 'true');
					return false;
				}
			})
			.mouseout(function(){
				$(this).attr('data-click', 'false');
			});
	}
})(jQuery);