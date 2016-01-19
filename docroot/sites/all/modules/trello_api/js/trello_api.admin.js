jQuery(document).ready(function () {
  if (location.hash != '') {
    window.location.replace(location.href.replace('#', '?'));
  }
});
