import QtQuick 2.0
import QtQuick.Controls 1.2

Item{
    property double maxValue;
    property double setValue;
    onSetValueChanged: {
        fadeDurSlider.setDur(setValue)
    }
    property double readValue;
    signal readValueActiveChanged();

     Slider{
         id:fadeDurSlider

         minimumValue: 0
         maximumValue: Math.log(parent.maxValue + 1)
         function setDur(dur){
             value = Math.log(dur + 1);
         }
         property double duration;

         onValueChanged: {
             duration =(Math.exp(value)-1);
         }

         function setValuePassive(val){
             supressChnage = true;
             value = val;
         }

         onDurationChanged: {
             if(pressed)
                 readValueActiveChanged();
             readValue = duration;
         }

         signal valueActivChanged();
     }
}
