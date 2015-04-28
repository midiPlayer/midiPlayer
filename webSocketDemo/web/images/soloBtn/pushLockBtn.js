/**
  data.on : callback function when slider is on
  data.off : callback function when slider is off
  data.type mute or solo
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
	$(this).animate({ left: "32"});
	$(this).addClass("locked");
	data.on();
      }
      else{//off
	$(this).animate({ left: "0"});
	$(this).removeClass("locked");
	data.off();
      }
      }});
  slider.mousedown(function(){$(this).addClass("active"); data.on()});
  slider.mouseup(function(){$(this).removeClass("active"); if(!$(this).hasClass("locked")) data.off();});
  slider.mouseleave(function(){$(this).removeClass("active"); if(!$(this).hasClass("locked")&& $(this).hasClass("active")) data.off();});
};
})(jQuery);
 
