#include"Pedestrian.h"


Pedestrian::Pedestrian():
	sonarSensor(PORT_3),touchSensor(PORT_1){
}
void Pedestrian::monitor(){
	int Distance;
	while(1){
		Distance=sonarSensor.getDistance();
		if(Distance<=40){
			flag=1;
			while(sonarSensor.getDistance()<=40){
			}
			clock.sleep(1000);
			break;
		}
	}
}

void Pedestrian::cross(){
	if(flag==1){
		walker.reset();
		lifter.liftUp();
		walker.run(30,0);
		clock.sleep(2000);
		walker.run(0,0);
		lifter.defaultSet(0);
		flag=2;
	}else{
		while(1){
			emoter.turn(30);
		}
	}
}	



