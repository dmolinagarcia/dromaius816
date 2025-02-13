# Dromaius816 Signal History

signalHistory is used to feed the Logic Analyzer. Each device that wants to track its signals should have an entry on signal_history_profiles.c

void dev_commodore_pet_history_profiles(struct DevCommodorePet *pet, const char *chip_name, struct SignalHistory *history) { }

This function must be called from the device creation function right after the call to `simulator_device_complete`. The call to `simulator_device_complete`creates the signal_history. This will create signal_groups which can be loaded all at once.

A signal must be given a name, within the device with signal_set_name.

> 	signal_set_name(SIGNAL_POOL, SIGNAL(CLOCK), "CLK");

## Add signal groups in device<dev>.h

```
typedef struct DevCommodorePet {
    ....
    signal_group_destroy(device->sg_data);
    ....
}
```

## Create the signals in the device_create function

```
	SIGNAL_GROUP_NEW_N(cpu_data, 8, &SIGNAL(D0), "cpu_data", "D%d");
```

## Destroy the group in the device destroy function

Before destroying the simulator

```
	signal_group_destroy(device->sg_data);
```

## Create the history profile

In signal_history_profiles.c, add the history_profile_Create for the device

```
    void dev_minimal_65816_history_profiles(struct DevMinimal65816 *device, const char *chip_name, struct SignalHistory *history) {

    	uint32_t prof_data = signal_history_profile_create(history, chip_name, "Data Bus");
    	for (size_t i = 0; i < arrlenu(device->sg_cpu_data); ++i) {
    		signal_history_profile_add_signal(history, prof_data, *device->sg_data[i], NULL);
    	}

    }
```

Add the function declaration to the header, and the struct

```
    struct DevMinimal65816;
```

## Includes

Add in the device<dev>.c the profiles.h

#include "signal_history_profiles.h"

Add in the profiles.c the device<dev>.h












