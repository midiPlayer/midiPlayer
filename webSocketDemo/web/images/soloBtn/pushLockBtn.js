/**
  data.on : callback function when slider is on
  data.off : callback function when slider is off
  data.type mute or solo
  
  to set chn chnge set Value of the btn use:
  $("#soloBtn").pushLockBtnOn(ui);
  $("#soloBtn").pushLockBtnOff(ui);
*/
(function($) {
$.fn.pushLockBtn = function(data){
  this.addClass("pushLockBtn");
  
  this.html("<div class='slider'></div>");
  
  if(data.type == "mute")
    this.addClass("mute");
  else
    this.addClass("solo");
  
  if(data.on == null){
    data.on = function(){};
  }
  if(data.off == null){
    data.off = function(){};
  }

  var slider = this.find(":first-child");
  slider.draggable({ axis: "x", containment: this, scroll: false ,drag: function(event,ui){
  if(ui.position.left > 15)
    $(this).addClass("locked");
  else
    $(this).removeClass("locked");
  },stop:
    function(event,ui){
      if(ui.position.left > 15){//on
	$(this).animate({ left: "31"});
	$(this).addClass("locked");
	data.on($(this).parent());
      }
      else{//off
	$(this).animate({ left: "0"});
	$(this).removeClass("locked");
	data.off($(this).parent());
      }
      }});
  slider.mousedown(function(e){if($(this).parent().hasClass("grayscale")) return; $(this).addClass("active");  data.on($(this).parent());});
  slider.mouseup(function(e){if($(this).parent().hasClass("grayscale")) return; $(this).removeClass("active"); if(!$(this).hasClass("locked")) data.off($(this).parent());});
  slider.mouseleave(function(e){if($(this).parent().hasClass("grayscale")) return; $(this).removeClass("active"); if(!$(this).hasClass("locked")&& $(this).hasClass("active")) data.off($(this).parent()); });
};

$.fn.pushLockBtnOn = function(){
   var slider = this.find(":first-child");
   slider.animate({ left: "31"});
   slider.addClass("locked");
};

$.fn.pushLockBtnOff = function(){
   var slider = this.find(":first-child");
   slider.animate({ left: "0"});
   slider.removeClass("locked");
   slider.removeClass("active");
};

$.fn.pushLockBtnDissable = function(){
   var slider = this.find(":first-child");
   slider.draggable( 'disable' );
   $(this).addClass("grayscale");
};

$.fn.pushLockBtnEnable = function(){
   var slider = this.find(":first-child");
   slider.draggable( 'enable' );
   $(this).removeClass("grayscale");
};



})(jQuery);
 
