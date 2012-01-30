
(function ($) {
  $(document).ready(function() {
    //var sig = Drupal.settings.signaturefield.sig || null;
    var api = $('form .sig').parents('form').signaturePad(Drupal.settings.signaturefield.settings);
    //regenerate existing signature when editing content
    setTimeout(function(){ api.regenerate(Drupal.settings.signaturefield.sig) }, 60);
  });
})(jQuery);
