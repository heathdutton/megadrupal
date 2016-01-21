/**
 * Call savvior on document ready with a sample configuration.
 */
jQuery(document).ready(function() {
  savvior.init(".svr-grid", {
    "screen and (max-width: 768px)": { columns: 2 },
    "screen and (min-width: 768px) and (max-width: 992px)": { columns: 3 },
    "screen and (min-width: 992px)": { columns: 4 },
  });
});
