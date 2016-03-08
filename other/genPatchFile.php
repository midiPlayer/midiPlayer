<?php
$devs="";
$cables="";
for($i = 1; $i  < 100; $i++){
    $client = "MidiPlayer-" . str_pad($i, 2 ,'0', STR_PAD_LEFT);
    $name = "midiPlayer" . $i;
    
    $devs .= '  <socket exclusive="off" client="' . $client . '" type="jack-audio" name="' . $name . '">
   <plug>out_audiosink-actual-sink-jackaudio_1</plug>
   <plug>out_audiosink-actual-sink-jackaudio_2</plug>
  </socket>' . "\n";
  
    $cables .= '<cable output="' . $name . '" input="aubio" type="jack-audio"/>'."\n";
}

$ret = '
<!DOCTYPE patchbay>
<patchbay version="0.4.0" name="patch1">
 <output-sockets>
  <socket exclusive="off" client="MidiPlayer" type="jack-audio" name="midiPlayer0">
   <plug>out_audiosink-actual-sink-jackaudio_1</plug>
   <plug>out_audiosink-actual-sink-jackaudio_2</plug>
  </socket>
' . $devs . '
 </output-sockets>
 <input-sockets>
  <socket exclusive="off" client="MidiPlayerInput" type="jack-audio" name="aubio">
   <plug>Audio Trigger in</plug>
  </socket>
 </input-sockets>
 <slots/>
 <cables>
  <cable output="midiPlayer0" input="aubio" type="jack-audio"/>
  ' . $cables . '
 </cables>
</patchbay>';

echo $ret;
?>
