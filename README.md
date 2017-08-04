# anashikiroot

This is "anashikiroot" to analyze SAMURAI experiment, based on "anaroot" by T. Isobe san.

Class "TArtShikiParameters" is a singleton manager for target information.
Please declare with the static function "TArtShikiParameters::Instance();", and load parameter file with LoadParameter(const char*) function.

Class "TArtEasyPID" is a singleton class for beam particle identification.
This class provides many useful functions, such as
Double_t TArtEasyPID::GetAoZ()
Double_t TArtEasyPID::GetZet()
Double_t TArtEasyPID::GetTOF713()
Double_t TArtEasyPID::GetKineticEnergy()
...
