/*
 * host_undefined_stubs.c - Auto-generated stubs for the PC port linker
 *
 * The decompiled arm9/src/*.c references many ARM-only symbols that are
 * normally resolved by the ARM linker script (config/undefined_syms.ld)
 * or come from the original ROM as raw assembly bytes. On the host we
 * cannot resolve them that way, so we provide weak stubs here just so
 * the program links and runs.
 *
 * REGENERATE WITH: python tools/scripts/gen_host_stubs.py
 * TRACE MODE: ON  (--trace adds first-call stderr logging)
 */
#include <stdint.h>
#include <stdio.h>

/* Suppress warnings about unused/empty stubs */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmain"

/* === DAT_* storage (1266 symbols) === */
uint32_t DAT_020073bc;
uint32_t DAT_020073c0;
uint32_t DAT_020073c4;
uint32_t DAT_020073c8;
uint32_t DAT_020073cc;
uint32_t DAT_02007478;
uint32_t DAT_0200747c;
uint32_t DAT_02007480;
uint32_t DAT_02007524;
uint32_t DAT_02007528;
uint32_t DAT_0200752c;
uint32_t DAT_02007584;
uint32_t DAT_02007588;
uint32_t DAT_0200758c;
uint32_t DAT_02007618;
uint32_t DAT_0200761c;
uint32_t DAT_02007620;
uint32_t DAT_02007624;
uint32_t DAT_02007628;
uint32_t DAT_02007640;
uint32_t DAT_02007644;
uint32_t DAT_0200780c;
uint32_t DAT_020079a4;
uint32_t DAT_020079a8;
uint32_t DAT_020079ac;
uint32_t DAT_020079b0;
uint32_t DAT_020163ac;
uint32_t DAT_020163b0;
uint32_t DAT_020163b4;
uint32_t DAT_02016570;
uint32_t DAT_02016574;
uint32_t DAT_02016578;
uint32_t DAT_0201657c;
uint32_t DAT_02016580;
uint32_t DAT_02016584;
uint32_t DAT_020169bc;
uint32_t DAT_02016bcc;
uint32_t DAT_02016bd0;
uint32_t DAT_02017980;
uint32_t DAT_02017984;
uint32_t DAT_02017b10;
uint32_t DAT_02017cfc;
uint32_t DAT_02017dcc;
uint32_t DAT_02018728;
uint32_t DAT_0201872c;
uint32_t DAT_02018730;
uint32_t DAT_02018734;
uint32_t DAT_02018738;
uint32_t DAT_0201873c;
uint32_t DAT_02018740;
uint32_t DAT_02018744;
uint32_t DAT_02018ab0;
uint32_t DAT_02018ab4;
uint32_t DAT_02018ab8;
uint32_t DAT_02018bb8;
uint32_t DAT_02018ca4;
uint32_t DAT_02018d0c;
uint32_t DAT_02018d10;
uint32_t DAT_02018e98;
uint32_t DAT_02018e9c;
uint32_t DAT_02018f08;
uint32_t DAT_02018f0c;
uint32_t DAT_020192d4;
uint32_t DAT_020192d8;
uint32_t DAT_020192dc;
uint32_t DAT_020192e0;
uint32_t DAT_020192e4;
uint32_t DAT_020192e8;
uint32_t DAT_020192ec;
uint32_t DAT_020192f0;
uint32_t DAT_020192f4;
uint32_t DAT_02019730;
uint32_t DAT_02019734;
uint32_t DAT_02019738;
uint32_t DAT_0201973c;
uint32_t DAT_02019740;
uint32_t DAT_02019744;
uint32_t DAT_02019748;
uint32_t DAT_0201974c;
uint32_t DAT_02019750;
uint32_t DAT_02019754;
uint32_t DAT_02019758;
uint32_t DAT_0201975c;
uint32_t DAT_02019760;
uint32_t DAT_02019764;
uint32_t DAT_02019768;
uint32_t DAT_0201976c;
uint32_t DAT_02019770;
uint32_t DAT_02019774;
uint32_t DAT_02019778;
uint32_t DAT_0201977c;
uint32_t DAT_02019780;
uint32_t DAT_02019784;
uint32_t DAT_02019788;
uint32_t DAT_0201978c;
uint32_t DAT_02019790;
uint32_t DAT_020197bc;
uint32_t DAT_02019b40;
uint32_t DAT_02019e98;
uint32_t DAT_0201a154;
uint32_t DAT_0201a158;
uint32_t DAT_0201a15c;
uint32_t DAT_0201a684;
uint32_t DAT_0201a688;
uint32_t DAT_0201a68c;
uint32_t DAT_0201a690;
uint32_t DAT_0201a694;
uint32_t DAT_0201abe0;
uint32_t DAT_0201abe4;
uint32_t DAT_0201abe8;
uint32_t DAT_0201abec;
uint32_t DAT_0201addc;
uint32_t DAT_0201ade0;
uint32_t DAT_0201ade4;
uint32_t DAT_0201ade8;
uint32_t DAT_0201adec;
uint32_t DAT_0201adf0;
uint32_t DAT_0201adf4;
uint32_t DAT_0201adf8;
uint32_t DAT_0201adfc;
uint32_t DAT_0201ae00;
uint32_t DAT_0201b300;
uint32_t DAT_0201b304;
uint32_t DAT_0201b308;
uint32_t DAT_0201b30c;
uint32_t DAT_0201b408;
uint32_t DAT_0201b40c;
uint32_t DAT_0201b410;
uint32_t DAT_0201b414;
uint32_t DAT_0201c6f4;
uint32_t DAT_0201d0bc;
uint32_t DAT_0201d0c0;
uint32_t DAT_0201d0c4;
uint32_t DAT_0201d0c8;
uint32_t DAT_0201d314;
uint32_t DAT_0201d9b4;
uint32_t DAT_0201d9b8;
uint32_t DAT_0201d9bc;
uint32_t DAT_0201d9c0;
uint32_t DAT_0201dcc8;
uint32_t DAT_0201dccc;
uint32_t DAT_0201dcd0;
uint32_t DAT_0201dcd4;
uint32_t DAT_0201e820;
uint32_t DAT_0201e824;
uint32_t DAT_0201e828;
uint32_t DAT_0201e82c;
uint32_t DAT_0201e830;
uint32_t DAT_0201e834;
uint32_t DAT_0201eedc;
uint32_t DAT_0201eee0;
uint32_t DAT_0201eee4;
uint32_t DAT_0201eee8;
uint32_t DAT_0201eeec;
uint32_t DAT_0201eef0;
uint32_t DAT_0201eef4;
uint32_t DAT_0201f04c;
uint32_t DAT_0201f1e0;
uint32_t DAT_0201f1e4;
uint32_t DAT_0201f1e8;
uint32_t DAT_0201f41c;
uint32_t DAT_0201f4e4;
uint32_t DAT_0201f5a4;
uint32_t DAT_0201fa10;
uint32_t DAT_0201fa14;
uint32_t DAT_0201fa18;
uint32_t DAT_0201fa1c;
uint32_t DAT_0201fa20;
uint32_t DAT_0201fa24;
uint32_t DAT_0201fa28;
uint32_t DAT_0201fa2c;
uint32_t DAT_0201fa30;
uint32_t DAT_0201fa34;
uint32_t DAT_0201fa38;
uint32_t DAT_0201fa3c;
uint32_t DAT_0201fa40;
uint32_t DAT_0201fa44;
uint32_t DAT_0201fa48;
uint32_t DAT_0201fa4c;
uint32_t DAT_0201feb8;
uint32_t DAT_0201febc;
uint32_t DAT_0201fec0;
uint32_t DAT_0201fec4;
uint32_t DAT_0201fec8;
uint32_t DAT_0201fecc;
uint32_t DAT_0201fed0;
uint32_t DAT_0201fed4;
uint32_t DAT_0201fed8;
uint32_t DAT_0201fedc;
uint32_t DAT_0201fee0;
uint32_t DAT_0201fee4;
uint32_t DAT_0201fee8;
uint32_t DAT_0201feec;
uint32_t DAT_0201fef0;
uint32_t DAT_0201fef4;
uint32_t DAT_02020290;
uint32_t DAT_02020294;
uint32_t DAT_020204b0;
uint32_t DAT_020204b4;
uint32_t DAT_020204b8;
uint32_t DAT_0202055c;
uint32_t DAT_02020858;
uint32_t DAT_0202085c;
uint32_t DAT_02020aac;
uint32_t DAT_02020ab0;
uint32_t DAT_02020ab4;
uint32_t DAT_02020da0;
uint32_t DAT_02020da4;
uint32_t DAT_02020da8;
uint32_t DAT_02020dac;
uint32_t DAT_02020db0;
uint32_t DAT_02020db4;
uint32_t DAT_02020f1c;
uint32_t DAT_02020f54;
uint32_t DAT_02020fe4;
uint32_t DAT_02021124;
uint32_t DAT_02021128;
uint32_t DAT_02021578;
uint32_t DAT_0202157c;
uint32_t DAT_02021580;
uint32_t DAT_02021584;
uint32_t DAT_020216e4;
uint32_t DAT_020216e8;
uint32_t DAT_020216ec;
uint32_t DAT_02021acc;
uint32_t DAT_02021ad0;
uint32_t DAT_02021ad4;
uint32_t DAT_02021ad8;
uint32_t DAT_02021c2c;
uint32_t DAT_02021c30;
uint32_t DAT_02021f54;
uint32_t DAT_02021f58;
uint32_t DAT_02021f5c;
uint32_t DAT_02021f60;
uint32_t DAT_02021f64;
uint32_t DAT_02021f68;
uint32_t DAT_02021f6c;
uint32_t DAT_02021f70;
uint32_t DAT_02021f74;
uint32_t DAT_02021f78;
uint32_t DAT_02024a98;
uint32_t DAT_02024a9c;
uint32_t DAT_02024aa0;
uint32_t DAT_02024aa4;
uint32_t DAT_02024aa8;
uint32_t DAT_02025184;
uint32_t DAT_02025188;
uint32_t DAT_0202518c;
uint32_t DAT_02025190;
uint32_t DAT_020257fc;
uint32_t DAT_02025800;
uint32_t DAT_02025804;
uint32_t DAT_02025808;
uint32_t DAT_0202580c;
uint32_t DAT_020265a8;
uint32_t DAT_02026794;
uint32_t DAT_02026798;
uint32_t DAT_0202679c;
uint32_t DAT_020267a0;
uint32_t DAT_020267a4;
uint32_t DAT_020267a8;
uint32_t DAT_020267ac;
uint32_t DAT_020267b0;
uint32_t DAT_020267b4;
uint32_t DAT_020267b8;
uint32_t DAT_020267bc;
uint32_t DAT_020267c0;
uint32_t DAT_020267c4;
uint32_t DAT_020267c8;
uint32_t DAT_020267cc;
uint32_t DAT_020267d0;
uint32_t DAT_020267d4;
uint32_t DAT_020267d8;
uint32_t DAT_020267dc;
uint32_t DAT_020267e0;
uint32_t DAT_020267e4;
uint32_t DAT_020267e8;
uint32_t DAT_020267ec;
uint32_t DAT_020267f0;
uint32_t DAT_020267f4;
uint32_t DAT_020267f8;
uint32_t DAT_020267fc;
uint32_t DAT_02026800;
uint32_t DAT_02026804;
uint32_t DAT_02026808;
uint32_t DAT_0202680c;
uint32_t DAT_02026810;
uint32_t DAT_02026814;
uint32_t DAT_02026818;
uint32_t DAT_0202681c;
uint32_t DAT_02026820;
uint32_t DAT_02026824;
uint32_t DAT_02026828;
uint32_t DAT_0202682c;
uint32_t DAT_02026860;
uint32_t DAT_02026864;
uint32_t DAT_02026b10;
uint32_t DAT_02026b14;
uint32_t DAT_02026be8;
uint32_t DAT_02026c68;
uint32_t DAT_02026c6c;
uint32_t DAT_02026ca8;
uint32_t DAT_02026cc8;
uint32_t DAT_02026ccc;
uint32_t DAT_02026e10;
uint32_t DAT_02027014;
uint32_t DAT_02027018;
uint32_t DAT_0202701c;
uint32_t DAT_020270c4;
uint32_t DAT_020270c8;
uint32_t DAT_020270cc;
uint32_t DAT_020270d0;
uint32_t DAT_0202728c;
uint32_t DAT_02027290;
uint32_t DAT_02027294;
uint32_t DAT_02027298;
uint32_t DAT_0202729c;
uint32_t DAT_02027368;
uint32_t DAT_0202736c;
uint32_t DAT_02027370;
uint32_t DAT_02027374;
uint32_t DAT_02027378;
uint32_t DAT_0202737c;
uint32_t DAT_020273f8;
uint32_t DAT_020273fc;
uint32_t DAT_02027400;
uint32_t DAT_020274fc;
uint32_t DAT_02027500;
uint32_t DAT_02027504;
uint32_t DAT_02027508;
uint32_t DAT_0202750c;
uint32_t DAT_02027510;
uint32_t DAT_020275d4;
uint32_t DAT_020275d8;
uint32_t DAT_020275dc;
uint32_t DAT_020275e0;
uint32_t DAT_0202764c;
uint32_t DAT_02027650;
uint32_t DAT_0202769c;
uint32_t DAT_0202770c;
uint32_t DAT_02027844;
uint32_t DAT_02027848;
uint32_t DAT_0202784c;
uint32_t DAT_02027850;
uint32_t DAT_020278f4;
uint32_t DAT_020278f8;
uint32_t DAT_02027a58;
uint32_t DAT_02027a5c;
uint32_t DAT_02027a60;
uint32_t DAT_02027a64;
uint32_t DAT_02027b28;
uint32_t DAT_02027b2c;
uint32_t DAT_02027b30;
uint32_t DAT_02027b34;
uint32_t DAT_02027c70;
uint32_t DAT_02027c74;
uint32_t DAT_02027cec;
uint32_t DAT_02027cf0;
uint32_t DAT_02027edc;
uint32_t DAT_02027fe8;
uint32_t DAT_02027fec;
uint32_t DAT_02027ff0;
uint32_t DAT_02028694;
uint32_t DAT_02028698;
uint32_t DAT_0202869c;
uint32_t DAT_020286a0;
uint32_t DAT_020286a4;
uint32_t DAT_020286a8;
uint32_t DAT_020286ac;
uint32_t DAT_020289e4;
uint32_t DAT_020289e8;
uint32_t DAT_020289ec;
uint32_t DAT_020289f0;
uint32_t DAT_02028aac;
uint32_t DAT_02028ab0;
uint32_t DAT_02028b00;
uint32_t DAT_02028b48;
uint32_t DAT_02028b4c;
uint32_t DAT_02028bcc;
uint32_t DAT_02028c1c;
uint32_t DAT_02028cd4;
uint32_t DAT_02028cd8;
uint32_t DAT_02028e98;
uint32_t DAT_02028e9c;
uint32_t DAT_02028ea0;
uint32_t DAT_02028ea4;
uint32_t DAT_020293b0;
uint32_t DAT_020293b4;
uint32_t DAT_020293b8;
uint32_t DAT_020293bc;
uint32_t DAT_020293c0;
uint32_t DAT_020293c4;
uint32_t DAT_02029478;
uint32_t DAT_0202947c;
uint32_t DAT_02029480;
uint32_t DAT_02029484;
uint32_t DAT_02029720;
uint32_t DAT_02029758;
uint32_t DAT_0202975c;
uint32_t DAT_02029760;
uint32_t DAT_02029764;
uint32_t DAT_02029768;
uint32_t DAT_02029838;
uint32_t DAT_0202983c;
uint32_t DAT_02029840;
uint32_t DAT_02029958;
uint32_t DAT_0202995c;
uint32_t DAT_02029960;
uint32_t DAT_02029df0;
uint32_t DAT_02029df4;
uint32_t DAT_02029df8;
uint32_t DAT_02029dfc;
uint32_t DAT_02029e00;
uint32_t DAT_02029e04;
uint32_t DAT_02029e08;
uint32_t DAT_02029e0c;
uint32_t DAT_02029e10;
uint32_t DAT_02029e14;
uint32_t DAT_02029e18;
uint32_t DAT_02029e1c;
uint32_t DAT_02029f84;
uint32_t DAT_02029f88;
uint32_t DAT_02029f8c;
uint32_t DAT_02029ff8;
uint32_t DAT_0202a174;
uint32_t DAT_0202a178;
uint32_t DAT_0202a17c;
uint32_t DAT_0202a180;
uint32_t DAT_0202a184;
uint32_t DAT_0202a510;
uint32_t DAT_0202a514;
uint32_t DAT_0202a518;
uint32_t DAT_0202a568;
uint32_t DAT_0202a6b4;
uint32_t DAT_0202a6b8;
uint32_t DAT_0202a6bc;
uint32_t DAT_0202a6c0;
uint32_t DAT_0202a6c4;
uint32_t DAT_0202a9f0;
uint32_t DAT_0202aaac;
uint32_t DAT_0202ac2c;
uint32_t DAT_0202af10;
uint32_t DAT_0202b1dc;
uint32_t DAT_0202b1e0;
uint32_t DAT_0202b1e4;
uint32_t DAT_0202b1e8;
uint32_t DAT_0202b1ec;
uint32_t DAT_0202b1f0;
uint32_t DAT_0202b1f4;
uint32_t DAT_0202b1f8;
uint32_t DAT_0202b1fc;
uint32_t DAT_0202b200;
uint32_t DAT_0202b204;
uint32_t DAT_0202b6ac;
uint32_t DAT_0202b6c0;
uint32_t DAT_0202b71c;
uint32_t DAT_0202b898;
uint32_t DAT_0202b89c;
uint32_t DAT_0202b920;
uint32_t DAT_0202b924;
uint32_t DAT_0202b928;
uint32_t DAT_0202b940;
uint32_t DAT_0202bcb0;
uint32_t DAT_0202bcb4;
uint32_t DAT_0202bcfc;
uint32_t DAT_0202bd4c;
uint32_t DAT_0202bd94;
uint32_t DAT_0202bea0;
uint32_t DAT_0202bea4;
uint32_t DAT_0202bff4;
uint32_t DAT_0202c0cc;
uint32_t DAT_0202c100;
uint32_t DAT_0202c104;
uint32_t DAT_0202c108;
uint32_t DAT_0202c77c;
uint32_t DAT_0202c780;
uint32_t DAT_0202c7d0;
uint32_t DAT_0202c828;
uint32_t DAT_0202c85c;
uint32_t DAT_0202c904;
uint32_t DAT_0202c9bc;
uint32_t DAT_0202cadc;
uint32_t DAT_0202cae0;
uint32_t DAT_0202cb0c;
uint32_t DAT_0202cb80;
uint32_t DAT_0202d158;
uint32_t DAT_0202dce0;
uint32_t DAT_0202dec0;
uint32_t DAT_0202dffc;
uint32_t DAT_0202e000;
uint32_t DAT_0202e004;
uint32_t DAT_0202e008;
uint32_t DAT_0202e00c;
uint32_t DAT_0202e01c;
uint32_t DAT_0202e070;
uint32_t DAT_0202e074;
uint32_t DAT_0202e084;
uint32_t DAT_0202e0d0;
uint32_t DAT_0202e0d4;
uint32_t DAT_0202e25c;
uint32_t DAT_0202e260;
uint32_t DAT_0202e264;
uint32_t DAT_0202e268;
uint32_t DAT_0202e26c;
uint32_t DAT_0202e270;
uint32_t DAT_0202e304;
uint32_t DAT_0202e308;
uint32_t DAT_0202e30c;
uint32_t DAT_0202e310;
uint32_t DAT_0202e320;
uint32_t DAT_0202e354;
uint32_t DAT_0202e358;
uint32_t DAT_0202e3b8;
uint32_t DAT_0202e3bc;
uint32_t DAT_0202e68c;
uint32_t DAT_0202e690;
uint32_t DAT_0202e6bc;
uint32_t DAT_0202ea54;
uint32_t DAT_0202eb60;
uint32_t DAT_0202eb64;
uint32_t DAT_0202eb68;
uint32_t DAT_0202eb6c;
uint32_t DAT_0202eb70;
uint32_t DAT_0202eb74;
uint32_t DAT_0202eb78;
uint32_t DAT_0202eb9c;
uint32_t DAT_0202eba0;
uint32_t DAT_0202eba4;
uint32_t DAT_0202ebc4;
uint32_t DAT_0202ec24;
uint32_t DAT_0202ec48;
uint32_t DAT_0202ec80;
uint32_t DAT_0202ecd4;
uint32_t DAT_0202ed38;
uint32_t DAT_0202ed94;
uint32_t DAT_0202ee20;
uint32_t DAT_0202ee24;
uint32_t DAT_0202ee9c;
uint32_t DAT_0202eea0;
uint32_t DAT_0202eedc;
uint32_t DAT_0202ef3c;
uint32_t DAT_0202f040;
uint32_t DAT_0202f0fc;
uint32_t DAT_0202f1c4;
uint32_t DAT_0202f330;
uint32_t DAT_0202f334;
uint32_t DAT_0202f338;
uint32_t DAT_0202f3e4;
uint32_t DAT_0202f3e8;
uint32_t DAT_0202f3ec;
uint32_t DAT_0202f3f0;
uint32_t DAT_0202f490;
uint32_t DAT_0202f58c;
uint32_t DAT_0202f620;
uint32_t DAT_0202f688;
uint32_t DAT_0202f698;
uint32_t DAT_0202f734;
uint32_t DAT_0202f738;
uint32_t DAT_0202f770;
uint32_t DAT_0202f8f0;
uint32_t DAT_0202f8f4;
uint32_t DAT_0202f938;
uint32_t DAT_0202fa70;
uint32_t DAT_0202fcc4;
uint32_t DAT_0202fcc8;
uint32_t DAT_0202fccc;
uint32_t DAT_0202fdd8;
uint32_t DAT_0202fddc;
uint32_t DAT_0202fde0;
uint32_t DAT_0202fde4;
uint32_t DAT_0202fe50;
uint32_t DAT_0202ff48;
uint32_t DAT_0202ff4c;
uint32_t DAT_0202ff50;
uint32_t DAT_0202ff54;
uint32_t DAT_02030054;
uint32_t DAT_0203014c;
uint32_t DAT_02030150;
uint32_t DAT_020304b0;
uint32_t DAT_020304b4;
uint32_t DAT_020304b8;
uint32_t DAT_0203060c;
uint32_t DAT_02030610;
uint32_t DAT_02030614;
uint32_t DAT_02030618;
uint32_t DAT_0203061c;
uint32_t DAT_02030620;
uint32_t DAT_02030624;
uint32_t DAT_0203066c;
uint32_t DAT_0203072c;
uint32_t DAT_020308d0;
uint32_t DAT_020309a4;
uint32_t DAT_02030a08;
uint32_t DAT_02030a6c;
uint32_t DAT_02030ad0;
uint32_t DAT_02030b34;
uint32_t DAT_02030c60;
uint32_t DAT_02030cd4;
uint32_t DAT_02030cec;
uint32_t DAT_02030f00;
uint32_t DAT_02030f04;
uint32_t DAT_02030f08;
uint32_t DAT_02030f90;
uint32_t DAT_02031054;
uint32_t DAT_02031550;
uint32_t DAT_02031810;
uint32_t DAT_02031814;
uint32_t DAT_02031890;
uint32_t DAT_0203190c;
uint32_t DAT_02031988;
uint32_t DAT_02031a04;
uint32_t DAT_02031cc0;
uint32_t DAT_02032380;
uint32_t DAT_02032384;
uint32_t DAT_02032388;
uint32_t DAT_02032c2c;
uint32_t DAT_02032c30;
uint32_t DAT_02032c34;
uint32_t DAT_02032c38;
uint32_t DAT_02032c3c;
uint32_t DAT_02032d88;
uint32_t DAT_02032ecc;
uint32_t DAT_02032ed0;
uint32_t DAT_02032ed4;
uint32_t DAT_02032ed8;
uint32_t DAT_02032edc;
uint32_t DAT_02032f80;
uint32_t DAT_02032f84;
uint32_t DAT_02032f88;
uint32_t DAT_02032fc0;
uint32_t DAT_0203300c;
uint32_t DAT_02033140;
uint32_t DAT_02033144;
uint32_t DAT_02033148;
uint32_t DAT_0203314c;
uint32_t DAT_0203323c;
uint32_t DAT_02033240;
uint32_t DAT_02033244;
uint32_t DAT_020332dc;
uint32_t DAT_020332e0;
uint32_t DAT_020332e4;
uint32_t DAT_02033618;
uint32_t DAT_0203361c;
uint32_t DAT_02033704;
uint32_t DAT_020339f4;
uint32_t DAT_020339f8;
uint32_t DAT_02033ae0;
uint32_t DAT_02033ae4;
uint32_t DAT_02033ae8;
uint32_t DAT_02033aec;
uint32_t DAT_02033af0;
uint32_t DAT_02033af4;
uint32_t DAT_02033af8;
uint32_t DAT_02033afc;
uint32_t DAT_02033d48;
uint32_t DAT_02033eec;
uint32_t DAT_020347f0;
uint32_t DAT_020347f4;
uint32_t DAT_020347f8;
uint32_t DAT_020347fc;
uint32_t DAT_0203482c;
uint32_t DAT_02034830;
uint32_t DAT_02034834;
uint32_t DAT_0203485c;
uint32_t DAT_02034860;
uint32_t DAT_020348ac;
uint32_t DAT_020348b0;
uint32_t DAT_020348e4;
uint32_t DAT_020348e8;
uint32_t DAT_020348ec;
uint32_t DAT_02034924;
uint32_t DAT_02034928;
uint32_t DAT_0203494c;
uint32_t DAT_02034950;
uint32_t DAT_020349c0;
uint32_t DAT_020349c4;
uint32_t DAT_02034a10;
uint32_t DAT_02034a14;
uint32_t DAT_02034b74;
uint32_t DAT_02034b78;
uint32_t DAT_02034b7c;
uint32_t DAT_02034b80;
uint32_t DAT_02034b84;
uint32_t DAT_02034b88;
uint32_t DAT_02034b8c;
uint32_t DAT_02034e2c;
uint32_t DAT_02034e30;
uint32_t DAT_02034e34;
uint32_t DAT_02034e38;
uint32_t DAT_02034e3c;
uint32_t DAT_02034e40;
uint32_t DAT_02034e44;
uint32_t DAT_02034e48;
uint32_t DAT_0203501c;
uint32_t DAT_02035108;
uint32_t DAT_02035210;
uint32_t DAT_02035214;
uint32_t DAT_02035258;
uint32_t DAT_0203525c;
uint32_t DAT_020355c0;
uint32_t DAT_020355d0;
uint32_t DAT_0203580c;
uint32_t DAT_02035810;
uint32_t DAT_02035814;
uint32_t DAT_02035834;
uint32_t DAT_02035838;
uint32_t DAT_0203583c;
uint32_t DAT_0203584c;
uint32_t DAT_02035850;
uint32_t DAT_02035860;
uint32_t DAT_02035864;
uint32_t DAT_02035884;
uint32_t DAT_02035888;
uint32_t DAT_020358a8;
uint32_t DAT_020358ac;
uint32_t DAT_02035968;
uint32_t DAT_0203596c;
uint32_t DAT_02035970;
uint32_t DAT_020359f4;
uint32_t DAT_020359f8;
uint32_t DAT_020359fc;
uint32_t DAT_02035a70;
uint32_t DAT_02035a74;
uint32_t DAT_02035a78;
uint32_t DAT_02035b18;
uint32_t DAT_02035b1c;
uint32_t DAT_02035b20;
uint32_t DAT_02035b24;
uint32_t DAT_02035c2c;
uint32_t DAT_02035c30;
uint32_t DAT_02035c34;
uint32_t DAT_02035c38;
uint32_t DAT_02035e24;
uint32_t DAT_02035e28;
uint32_t DAT_02035e2c;
uint32_t DAT_02035e30;
uint32_t DAT_02035e34;
uint32_t DAT_02035e38;
uint32_t DAT_02035e3c;
uint32_t DAT_02035eec;
uint32_t DAT_02035ef0;
uint32_t DAT_02035ef4;
uint32_t DAT_02035ffc;
uint32_t DAT_02036000;
uint32_t DAT_02036004;
uint32_t DAT_02036008;
uint32_t DAT_02036160;
uint32_t DAT_02036164;
uint32_t DAT_02036168;
uint32_t DAT_0203616c;
uint32_t DAT_02036170;
uint32_t DAT_02036174;
uint32_t DAT_02036410;
uint32_t DAT_02036414;
uint32_t DAT_02036418;
uint32_t DAT_0203641c;
uint32_t DAT_02036420;
uint32_t DAT_02036424;
uint32_t DAT_02036428;
uint32_t DAT_0203642c;
uint32_t DAT_020364c4;
uint32_t DAT_020364c8;
uint32_t DAT_020364cc;
uint32_t DAT_020364d0;
uint32_t DAT_020364d4;
uint32_t DAT_020364d8;
uint32_t DAT_020364dc;
uint32_t DAT_020364e0;
uint32_t DAT_020364e4;
uint32_t DAT_02036594;
uint32_t DAT_02036634;
uint32_t DAT_02036654;
uint32_t DAT_020366a8;
uint32_t DAT_02036758;
uint32_t DAT_0203675c;
uint32_t DAT_020367e8;
uint32_t DAT_02036864;
uint32_t DAT_02036868;
uint32_t DAT_020368f4;
uint32_t DAT_02036bf0;
uint32_t DAT_02036c1c;
uint32_t DAT_02036cd8;
uint32_t DAT_02036cdc;
uint32_t DAT_02036d10;
uint32_t DAT_02036d14;
uint32_t DAT_020374f4;
uint32_t DAT_020374f8;
uint32_t DAT_020377e8;
uint32_t DAT_020377ec;
uint32_t DAT_020377f0;
uint32_t DAT_020377f4;
uint32_t DAT_020379d8;
uint32_t DAT_020379dc;
uint32_t DAT_020379e0;
uint32_t DAT_020379e4;
uint32_t DAT_02037a2c;
uint32_t DAT_02037a30;
uint32_t DAT_02037aa0;
uint32_t DAT_02037aa4;
uint32_t DAT_02037ac8;
uint32_t DAT_02037bfc;
uint32_t DAT_02037c00;
uint32_t DAT_02037c04;
uint32_t DAT_02037c74;
uint32_t DAT_02037c78;
uint32_t DAT_02037cdc;
uint32_t DAT_02037ce0;
uint32_t DAT_02037ce4;
uint32_t DAT_02037ce8;
uint32_t DAT_020385b0;
uint32_t DAT_02038610;
uint32_t DAT_02038724;
uint32_t DAT_02038728;
uint32_t DAT_0203872c;
uint32_t DAT_0203879c;
uint32_t DAT_020387a0;
uint32_t DAT_020387e0;
uint32_t DAT_020387e4;
uint32_t DAT_020387e8;
uint32_t DAT_02038848;
uint32_t DAT_0203884c;
uint32_t DAT_02038850;
uint32_t DAT_02038854;
uint32_t DAT_02038858;
uint32_t DAT_020389b0;
uint32_t DAT_020389b4;
uint32_t DAT_020389b8;
uint32_t DAT_020389bc;
uint32_t DAT_02038a24;
uint32_t DAT_02038a28;
uint32_t DAT_02038a2c;
uint32_t DAT_02038a30;
uint32_t DAT_02038a34;
uint32_t DAT_02038a38;
uint32_t DAT_02038a3c;
uint32_t DAT_02038e54;
uint32_t DAT_02038e58;
uint32_t DAT_02038e6c;
uint32_t DAT_02038f10;
uint32_t DAT_02038fc8;
uint32_t DAT_02038fcc;
uint32_t DAT_02038fd0;
uint32_t DAT_020390a0;
uint32_t DAT_02039140;
uint32_t DAT_02039274;
uint32_t DAT_02039278;
uint32_t DAT_0203927c;
uint32_t DAT_02039280;
uint32_t DAT_02039284;
uint32_t DAT_02039288;
uint32_t DAT_0203938c;
uint32_t DAT_02039390;
uint32_t DAT_02039464;
uint32_t DAT_02039468;
uint32_t DAT_0203946c;
uint32_t DAT_02039470;
uint32_t DAT_02039544;
uint32_t DAT_020395ac;
uint32_t DAT_02039604;
uint32_t DAT_02039608;
uint32_t DAT_02039704;
uint32_t DAT_02039708;
uint32_t DAT_0203970c;
uint32_t DAT_02039710;
uint32_t DAT_02039858;
uint32_t DAT_0203985c;
uint32_t DAT_02039860;
uint32_t DAT_02039864;
uint32_t DAT_02039868;
uint32_t DAT_0203986c;
uint32_t DAT_02039870;
uint32_t DAT_02039874;
uint32_t DAT_02039878;
uint32_t DAT_0203987c;
uint32_t DAT_02039880;
uint32_t DAT_02039884;
uint32_t DAT_02039888;
uint32_t DAT_0203988c;
uint32_t DAT_02039890;
uint32_t DAT_020398d8;
uint32_t DAT_02039940;
uint32_t DAT_02039a90;
uint32_t DAT_02039a94;
uint32_t DAT_02039a98;
uint32_t DAT_02039b60;
uint32_t DAT_02039b64;
uint32_t DAT_02039b68;
uint32_t DAT_02039b6c;
uint32_t DAT_02039be0;
uint32_t DAT_02039be4;
uint32_t DAT_02039e44;
uint32_t DAT_02039f04;
uint32_t DAT_02039f90;
uint32_t DAT_0203a1e8;
uint32_t DAT_0203a1ec;
uint32_t DAT_0203a1f0;
uint32_t DAT_0203a1f4;
uint32_t DAT_0203a1f8;
uint32_t DAT_0203a1fc;
uint32_t DAT_0203a30c;
uint32_t DAT_0203a310;
uint32_t DAT_0203a314;
uint32_t DAT_0203a318;
uint32_t DAT_0203a31c;
uint32_t DAT_0203a320;
uint32_t DAT_0203a324;
uint32_t DAT_0203a328;
uint32_t DAT_0203a3b0;
uint32_t DAT_0203a3b4;
uint32_t DAT_0203a3b8;
uint32_t DAT_0203a3bc;
uint32_t DAT_0203a4d4;
uint32_t DAT_0203a698;
uint32_t DAT_0203a69c;
uint32_t DAT_0203a6a0;
uint32_t DAT_0203a6a4;
uint32_t DAT_0203a724;
uint32_t DAT_0203a728;
uint32_t DAT_0203a72c;
uint32_t DAT_0203a730;
uint32_t DAT_0203a734;
uint32_t DAT_0203a7f4;
uint32_t DAT_0203a7f8;
uint32_t DAT_0203a7fc;
uint32_t DAT_0203a800;
uint32_t DAT_0203a878;
uint32_t DAT_0203a87c;
uint32_t DAT_0203a880;
uint32_t DAT_0203a884;
uint32_t DAT_0203a888;
uint32_t DAT_0203a910;
uint32_t DAT_0203a914;
uint32_t DAT_0203a918;
uint32_t DAT_0203a91c;
uint32_t DAT_0203a920;
uint32_t DAT_0203a924;
uint32_t DAT_0203aa44;
uint32_t DAT_0203aa48;
uint32_t DAT_0203aa4c;
uint32_t DAT_0203ac9c;
uint32_t DAT_0203ada4;
uint32_t DAT_0203ada8;
uint32_t DAT_0203adac;
uint32_t DAT_0203adb0;
uint32_t DAT_0203ae14;
uint32_t DAT_0203ae18;
uint32_t DAT_0203ae1c;
uint32_t DAT_0203ae20;
uint32_t DAT_0203b01c;
uint32_t DAT_0203b020;
uint32_t DAT_0203b0b0;
uint32_t DAT_0203b0c8;
uint32_t DAT_0203b0cc;
uint32_t DAT_0203b154;
uint32_t DAT_0203b158;
uint32_t DAT_0203b15c;
uint32_t DAT_0203b2c4;
uint32_t DAT_0203b344;
uint32_t DAT_0203b348;
uint32_t DAT_0203b3b4;
uint32_t DAT_0203b3b8;
uint32_t DAT_0203b538;
uint32_t DAT_0203b5b0;
uint32_t DAT_0203b6b8;
uint32_t DAT_0203b6bc;
uint32_t DAT_0203b730;
uint32_t DAT_0203b7a4;
uint32_t DAT_0203bd7c;
uint32_t DAT_0203bd80;
uint32_t DAT_0203c38c;
uint32_t DAT_0203c3e0;
uint32_t DAT_0203c3e4;
uint32_t DAT_0203c484;
uint32_t DAT_0203c514;
uint32_t DAT_0203c554;
uint32_t DAT_0203c87c;
uint32_t DAT_0203c880;
uint32_t DAT_0203cb34;
uint32_t DAT_0203cb38;
uint32_t DAT_0203cb3c;
uint32_t DAT_0203cb40;
uint32_t DAT_0203cb44;
uint32_t DAT_0203cb48;
uint32_t DAT_0203cb4c;
uint32_t DAT_0203cb50;
uint32_t DAT_0203cb54;
uint32_t DAT_0203cb58;
uint32_t DAT_0203cb5c;
uint32_t DAT_0203cb60;
uint32_t DAT_0203cb64;
uint32_t DAT_0203cb68;
uint32_t DAT_0203cbc8;
uint32_t DAT_0203cbf8;
uint32_t DAT_0203cc48;
uint32_t DAT_0203ccfc;
uint32_t DAT_0203d50c;
uint32_t DAT_0203d510;
uint32_t DAT_0203d514;
uint32_t DAT_0203d5c4;
uint32_t DAT_0203d5c8;
uint32_t DAT_0203d5f0;
uint32_t DAT_0203d658;
uint32_t DAT_0203d65c;
uint32_t DAT_0203d750;
uint32_t DAT_0203d754;
uint32_t DAT_0203d758;
uint32_t DAT_0203d75c;
uint32_t DAT_0203d760;
uint32_t DAT_0203d764;
uint32_t DAT_0203d768;
uint32_t DAT_0203dbd4;
uint32_t DAT_0203df00;
uint32_t DAT_0203df98;
uint32_t DAT_0203e0a0;
uint32_t DAT_0203e2e0;
uint32_t DAT_0203e59c;
uint32_t DAT_0203e610;
uint32_t DAT_0203e614;
uint32_t DAT_0203e6d0;
uint32_t DAT_0203e6d4;
uint32_t DAT_0203ec80;
uint32_t DAT_0203f29c;
uint32_t DAT_0203f550;
uint32_t DAT_0203f554;
uint32_t DAT_0203f558;
uint32_t DAT_0203f55c;
uint32_t DAT_0203f560;
uint32_t DAT_0203f564;
uint32_t DAT_0203f568;
uint32_t DAT_0203f56c;
uint32_t DAT_0203f578;
uint32_t DAT_0203f57c;
uint32_t DAT_0203f580;
uint32_t DAT_0203f584;
uint32_t DAT_0203f628;
uint32_t DAT_0203f678;
uint32_t DAT_0203f67c;
uint32_t DAT_0203fa00;
uint32_t DAT_0203fa04;
uint32_t DAT_0204062c;
uint32_t DAT_02040630;
uint32_t DAT_02040634;
uint32_t DAT_02040638;
uint32_t DAT_0204063c;
uint32_t DAT_020406a0;
uint32_t DAT_020406a4;
uint32_t DAT_020406a8;
uint32_t DAT_020406ac;
uint32_t DAT_020406b0;
uint32_t DAT_02040808;
uint32_t DAT_0204080c;
uint32_t DAT_02040a70;
uint32_t DAT_02040a74;
uint32_t DAT_02040a78;
uint32_t DAT_02040a7c;
uint32_t DAT_02040cc4;
uint32_t DAT_02040d64;
uint32_t DAT_02040e9c;
uint32_t DAT_02040ea0;
uint32_t DAT_02040ea4;
uint32_t DAT_02040ea8;
uint32_t DAT_02040eac;
uint32_t DAT_02040f58;
uint32_t DAT_02040fe0;
uint32_t DAT_02040fe4;
uint32_t DAT_02040fe8;
uint32_t DAT_020412a0;
uint32_t DAT_020412a4;
uint32_t DAT_020412a8;
uint32_t DAT_020414e4;
uint32_t DAT_020414e8;
uint32_t DAT_020414ec;
uint32_t DAT_02041704;
uint32_t DAT_02041708;
uint32_t DAT_0204170c;
uint32_t DAT_02041710;
uint32_t DAT_02041714;
uint32_t DAT_02041718;
uint32_t DAT_0204171c;
uint32_t DAT_02041720;
uint32_t DAT_02041724;
uint32_t DAT_02041808;
uint32_t DAT_0204185c;
uint32_t DAT_020418ec;
uint32_t DAT_02041928;
uint32_t DAT_020419cc;
uint32_t DAT_02041a38;
uint32_t DAT_02041a3c;
uint32_t DAT_02041a40;
uint32_t DAT_02041a44;
uint32_t DAT_02041aac;
uint32_t DAT_02041ab0;
uint32_t DAT_02041ab4;
uint32_t DAT_02041ab8;
uint32_t DAT_02041b5c;
uint32_t DAT_02041b60;
uint32_t DAT_02041b64;
uint32_t DAT_02041b68;
uint32_t DAT_02041b6c;
uint32_t DAT_02041b70;
uint32_t DAT_02041c4c;
uint32_t DAT_02041c50;
uint32_t DAT_02041c54;
uint32_t DAT_02041c58;
uint32_t DAT_02041c5c;
uint32_t DAT_02041d74;
uint32_t DAT_02041de4;
uint32_t DAT_02042414;
uint32_t DAT_02042418;
uint32_t DAT_0204241c;
uint32_t DAT_02042420;
uint32_t DAT_02042460;
uint32_t DAT_02042464;
uint32_t DAT_020424e0;
uint32_t DAT_02042560;
uint32_t DAT_02042564;
uint32_t DAT_02042568;
uint32_t DAT_020426a8;
uint32_t DAT_020427e8;
uint32_t DAT_020427ec;
uint32_t DAT_020427f0;
uint32_t DAT_020427f4;
uint32_t DAT_020427f8;
uint32_t DAT_020427fc;
uint32_t DAT_02042800;
uint32_t DAT_02042804;
uint32_t DAT_02042808;
uint32_t DAT_020429d4;
uint32_t DAT_020429d8;
uint32_t DAT_02042a2c;
uint32_t DAT_02042a7c;
uint32_t DAT_02042c48;
uint32_t DAT_02042c4c;
uint32_t DAT_02042c50;
uint32_t DAT_02042c54;
uint32_t DAT_02042d44;
uint32_t DAT_02042d48;
uint32_t DAT_02043338;
uint32_t DAT_02043494;
uint32_t DAT_0204358c;
uint32_t DAT_02043590;
uint32_t DAT_02043594;
uint32_t DAT_020436d8;
uint32_t DAT_020436dc;
uint32_t DAT_020436e0;
uint32_t DAT_020436e4;
uint32_t DAT_020436e8;
uint32_t DAT_0204375c;
uint32_t DAT_020437a8;
uint32_t DAT_020437ac;
uint32_t DAT_020437b0;
uint32_t DAT_020437b4;
uint32_t DAT_02043830;
uint32_t DAT_02043834;
uint32_t DAT_02043838;
uint32_t DAT_0204383c;
uint32_t DAT_02043840;
uint32_t DAT_02043844;
uint32_t DAT_02043848;
uint32_t DAT_0204384c;
uint32_t DAT_02043850;
uint32_t DAT_02043854;
uint32_t DAT_020438d4;
uint32_t DAT_0204396c;
uint32_t DAT_02043b4c;
uint32_t DAT_02043bb4;
uint32_t DAT_02043bb8;
uint32_t DAT_02043bec;
uint32_t DAT_02043bf0;
uint32_t DAT_02043cfc;
uint32_t DAT_02043d30;
uint32_t DAT_02043d78;
uint32_t DAT_02043e88;
uint32_t DAT_02043e8c;
uint32_t DAT_02043e90;
uint32_t DAT_02043e94;
uint32_t DAT_02043ed0;
uint32_t DAT_02043ed4;
uint32_t DAT_02043f48;
uint32_t DAT_02043f4c;
uint32_t DAT_02043fdc;
uint32_t DAT_02044018;
uint32_t DAT_020441c0;
uint32_t DAT_020441c4;
uint32_t DAT_020441c8;
uint32_t DAT_020441cc;
uint32_t DAT_020441d0;
uint32_t DAT_020441d4;
uint32_t DAT_020441d8;
uint32_t DAT_020441dc;
uint32_t DAT_020441e0;
uint32_t DAT_020441e4;
uint32_t DAT_02044348;
uint32_t DAT_02044464;
uint32_t DAT_02044468;
uint32_t DAT_0204446c;
uint32_t DAT_02044470;
uint32_t DAT_02044474;
uint32_t DAT_02044478;
uint32_t DAT_0204447c;
uint32_t DAT_020444cc;
uint32_t DAT_020444d0;
uint32_t DAT_02044500;
uint32_t DAT_02044570;
uint32_t DAT_02044884;
uint32_t DAT_02044888;
uint32_t DAT_0204488c;
uint32_t DAT_02044890;
uint32_t DAT_02044894;
uint32_t DAT_02044bac;
uint32_t DAT_02044fd8;
uint32_t DAT_020453a4;
uint32_t DAT_02046ee8;
uint32_t DAT_02046eec;
uint32_t DAT_02048550;
uint32_t DAT_02048614;
uint32_t DAT_02048c44;
uint32_t DAT_02048cb8;
uint32_t DAT_02059e04;
uint32_t DAT_02059e08;
uint32_t DAT_02059e0c;
uint32_t DAT_02059e10;
uint32_t DAT_02059e14;
uint32_t DAT_02059e18;
uint32_t DAT_02059f2c;
uint32_t DAT_02059f30;
uint32_t DAT_02059f34;
uint32_t DAT_02059f38;
uint32_t DAT_02059f3c;
uint32_t DAT_02059f40;
uint32_t DAT_02059f44;
uint32_t DAT_02059f48;
uint32_t DAT_02059f4c;
uint32_t DAT_02059f50;
uint32_t DAT_02059f54;
uint32_t DAT_02059f58;
uint32_t DAT_02059f5c;
uint32_t DAT_0205a020;
uint32_t DAT_0205a024;
uint32_t DAT_0205a028;
uint32_t DAT_0205a02c;
uint32_t DAT_0205a030;
uint32_t DAT_0205a034;
uint32_t DAT_0205a038;
uint32_t DAT_0205a03c;
uint32_t DAT_0205a078;
uint32_t DAT_0205a07c;
uint32_t DAT_0205a0d8;
uint32_t DAT_0205a0dc;
uint32_t DAT_0205a114;
uint32_t DAT_0205a184;
uint32_t DAT_0205a188;
uint32_t DAT_0205a1d8;
uint32_t DAT_0205a27c;
uint32_t DAT_0205a280;
uint32_t DAT_0205a284;
uint32_t DAT_040000b0;
uint32_t DAT_05000000;
uint32_t DAT_080000b2;
uint32_t _DAT_04000000;
uint32_t _DAT_040000b0;
uint32_t _DAT_04000208;
uint32_t _DAT_04100000;
uint32_t _DAT_08000000;
uint32_t _DAT_080000ac;
uint32_t _DAT_080000be;

/* === FUN_* stubs (318 symbols) === */
int FUN_02004228() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02004228\n");} return 0; }
int FUN_0200445a() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0200445a\n");} return 0; }
int FUN_02005380() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02005380\n");} return 0; }
int FUN_020064d0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020064d0\n");} return 0; }
int FUN_02007178() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02007178\n");} return 0; }
int FUN_02008534() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02008534\n");} return 0; }
int FUN_0200a2c4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0200a2c4\n");} return 0; }
int FUN_0200b278() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0200b278\n");} return 0; }
int FUN_0200c334() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0200c334\n");} return 0; }
int FUN_0200c3a0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0200c3a0\n");} return 0; }
int FUN_0200e640() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0200e640\n");} return 0; }
int FUN_0200eb10() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0200eb10\n");} return 0; }
int FUN_02010bfc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02010bfc\n");} return 0; }
int FUN_02017dd0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02017dd0\n");} return 0; }
int FUN_02018194() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02018194\n");} return 0; }
int FUN_02018bbc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02018bbc\n");} return 0; }
int FUN_020197e4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020197e4\n");} return 0; }
int FUN_0201a698() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0201a698\n");} return 0; }
int FUN_0201dcd8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0201dcd8\n");} return 0; }
int FUN_020200f4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020200f4\n");} return 0; }
int FUN_02021c34() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02021c34\n");} return 0; }
int FUN_02021c64() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02021c64\n");} return 0; }
int FUN_02022048() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02022048\n");} return 0; }
int FUN_02022200() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02022200\n");} return 0; }
int FUN_020222d8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020222d8\n");} return 0; }
int FUN_020222f4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020222f4\n");} return 0; }
int FUN_020223e8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020223e8\n");} return 0; }
int FUN_02022564() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02022564\n");} return 0; }
int FUN_0202277c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202277c\n");} return 0; }
int FUN_0202281c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202281c\n");} return 0; }
int FUN_02022868() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02022868\n");} return 0; }
int FUN_02023a78() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02023a78\n");} return 0; }
int FUN_02025810() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02025810\n");} return 0; }
int FUN_02025f20() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02025f20\n");} return 0; }
int FUN_0202605c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202605c\n");} return 0; }
int FUN_0202609c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202609c\n");} return 0; }
int FUN_0202626c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202626c\n");} return 0; }
int FUN_02026388() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02026388\n");} return 0; }
int FUN_02026868() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02026868\n");} return 0; }
int FUN_02027798() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02027798\n");} return 0; }
int FUN_02027bb8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02027bb8\n");} return 0; }
int FUN_02027cf4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02027cf4\n");} return 0; }
int FUN_02027e5c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02027e5c\n");} return 0; }
int FUN_02028028() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02028028\n");} return 0; }
int FUN_0202807c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202807c\n");} return 0; }
int FUN_02028128() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02028128\n");} return 0; }
int FUN_02028440() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02028440\n");} return 0; }
int FUN_02028710() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02028710\n");} return 0; }
int FUN_02028a30() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02028a30\n");} return 0; }
int FUN_02028b50() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02028b50\n");} return 0; }
int FUN_02028c20() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02028c20\n");} return 0; }
int FUN_0202916c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202916c\n");} return 0; }
int FUN_0202958c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202958c\n");} return 0; }
int FUN_02029964() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02029964\n");} return 0; }
int FUN_02029ab8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02029ab8\n");} return 0; }
int FUN_02029bf8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02029bf8\n");} return 0; }
int FUN_02029c1c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02029c1c\n");} return 0; }
int FUN_02029e20() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02029e20\n");} return 0; }
int FUN_02029ffc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02029ffc\n");} return 0; }
int FUN_0202a034() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202a034\n");} return 0; }
int FUN_0202a068() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202a068\n");} return 0; }
int FUN_0202a20c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202a20c\n");} return 0; }
int FUN_0202a5a8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202a5a8\n");} return 0; }
int FUN_0202a74c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202a74c\n");} return 0; }
int FUN_0202acdc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202acdc\n");} return 0; }
int FUN_0202b428() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202b428\n");} return 0; }
int FUN_0202b944() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202b944\n");} return 0; }
int FUN_0202b9b8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202b9b8\n");} return 0; }
int FUN_0202ba28() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202ba28\n");} return 0; }
int FUN_0202bacc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202bacc\n");} return 0; }
int FUN_0202bb54() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202bb54\n");} return 0; }
int FUN_0202bd98() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202bd98\n");} return 0; }
int FUN_0202cba8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202cba8\n");} return 0; }
int FUN_0202cc10() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202cc10\n");} return 0; }
int FUN_0202cc94() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202cc94\n");} return 0; }
int FUN_0202cd68() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202cd68\n");} return 0; }
int FUN_0202ce44() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202ce44\n");} return 0; }
int FUN_0202cec0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202cec0\n");} return 0; }
int FUN_0202cf40() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202cf40\n");} return 0; }
int FUN_0202cfbc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202cfbc\n");} return 0; }
int FUN_0202d964() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202d964\n");} return 0; }
int FUN_0202d9d0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202d9d0\n");} return 0; }
int FUN_0202da48() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202da48\n");} return 0; }
int FUN_0202dce4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202dce4\n");} return 0; }
int FUN_0202ebc8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202ebc8\n");} return 0; }
int FUN_0202f73c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0202f73c\n");} return 0; }
int FUN_020304bc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020304bc\n");} return 0; }
int FUN_0203056c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203056c\n");} return 0; }
int FUN_02030730() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02030730\n");} return 0; }
int FUN_02030790() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02030790\n");} return 0; }
int FUN_02030870() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02030870\n");} return 0; }
int FUN_0203088c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203088c\n");} return 0; }
int FUN_020308d4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020308d4\n");} return 0; }
int FUN_02030958() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02030958\n");} return 0; }
int FUN_02030b38() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02030b38\n");} return 0; }
int FUN_02030b9c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02030b9c\n");} return 0; }
int FUN_02030c64() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02030c64\n");} return 0; }
int FUN_02031058() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02031058\n");} return 0; }
int FUN_02031064() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02031064\n");} return 0; }
int FUN_020311a8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020311a8\n");} return 0; }
int FUN_02031218() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02031218\n");} return 0; }
int FUN_020312d8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020312d8\n");} return 0; }
int FUN_020312f4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020312f4\n");} return 0; }
int FUN_02031f20() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02031f20\n");} return 0; }
int FUN_02031f50() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02031f50\n");} return 0; }
int FUN_02032188() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02032188\n");} return 0; }
int FUN_02032220() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02032220\n");} return 0; }
int FUN_02033708() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02033708\n");} return 0; }
int FUN_02033f8c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02033f8c\n");} return 0; }
int FUN_02034084() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02034084\n");} return 0; }
int FUN_020340b8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020340b8\n");} return 0; }
int FUN_02034150() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02034150\n");} return 0; }
int FUN_02034168() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02034168\n");} return 0; }
int FUN_02034180() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02034180\n");} return 0; }
int FUN_020341bc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020341bc\n");} return 0; }
int FUN_020341ec() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020341ec\n");} return 0; }
int FUN_02034224() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02034224\n");} return 0; }
int FUN_02034274() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02034274\n");} return 0; }
int FUN_02035110() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02035110\n");} return 0; }
int FUN_020369c4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020369c4\n");} return 0; }
int FUN_02036c60() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02036c60\n");} return 0; }
int FUN_02036ca0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02036ca0\n");} return 0; }
int FUN_02037acc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02037acc\n");} return 0; }
int FUN_02037b18() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02037b18\n");} return 0; }
int FUN_02037b8c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02037b8c\n");} return 0; }
int FUN_02037cec() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02037cec\n");} return 0; }
int FUN_02037d50() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02037d50\n");} return 0; }
int FUN_02037dd0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02037dd0\n");} return 0; }
int FUN_02037ed4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02037ed4\n");} return 0; }
int FUN_02037f3c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02037f3c\n");} return 0; }
int FUN_02037fa4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02037fa4\n");} return 0; }
int FUN_0203800c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203800c\n");} return 0; }
int FUN_02038074() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038074\n");} return 0; }
int FUN_020380dc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020380dc\n");} return 0; }
int FUN_02038144() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038144\n");} return 0; }
int FUN_020381ac() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020381ac\n");} return 0; }
int FUN_02038214() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038214\n");} return 0; }
int FUN_0203827c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203827c\n");} return 0; }
int FUN_020382e4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020382e4\n");} return 0; }
int FUN_0203834c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203834c\n");} return 0; }
int FUN_020383b4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020383b4\n");} return 0; }
int FUN_0203841c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203841c\n");} return 0; }
int FUN_02038484() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038484\n");} return 0; }
int FUN_020384ec() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020384ec\n");} return 0; }
int FUN_02038614() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038614\n");} return 0; }
int FUN_0203867c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203867c\n");} return 0; }
int FUN_02038c4c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038c4c\n");} return 0; }
int FUN_02038c80() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038c80\n");} return 0; }
int FUN_02038cc8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038cc8\n");} return 0; }
int FUN_02038d00() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038d00\n");} return 0; }
int FUN_02038d34() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038d34\n");} return 0; }
int FUN_02038d80() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038d80\n");} return 0; }
int FUN_02038e7c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038e7c\n");} return 0; }
int FUN_02038f14() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038f14\n");} return 0; }
int FUN_02038f4c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038f4c\n");} return 0; }
int FUN_02038f68() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038f68\n");} return 0; }
int FUN_02038fd4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038fd4\n");} return 0; }
int FUN_02038ff4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02038ff4\n");} return 0; }
int FUN_020392a4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020392a4\n");} return 0; }
int FUN_020392d8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020392d8\n");} return 0; }
int FUN_02039474() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02039474\n");} return 0; }
int FUN_02039944() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02039944\n");} return 0; }
int FUN_020399d8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020399d8\n");} return 0; }
int FUN_02039a24() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02039a24\n");} return 0; }
int FUN_02039be8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02039be8\n");} return 0; }
int FUN_02039bf0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02039bf0\n");} return 0; }
int FUN_02039c9c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02039c9c\n");} return 0; }
int FUN_02039d64() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02039d64\n");} return 0; }
int FUN_02039d8c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02039d8c\n");} return 0; }
int FUN_02039db0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02039db0\n");} return 0; }
int FUN_02039f94() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02039f94\n");} return 0; }
int FUN_02039fe0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02039fe0\n");} return 0; }
int FUN_0203a014() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203a014\n");} return 0; }
int FUN_0203a030() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203a030\n");} return 0; }
int FUN_0203a04c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203a04c\n");} return 0; }
int FUN_0203a070() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203a070\n");} return 0; }
int FUN_0203a088() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203a088\n");} return 0; }
int FUN_0203a0f0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203a0f0\n");} return 0; }
int FUN_0203a104() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203a104\n");} return 0; }
int FUN_0203a4d8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203a4d8\n");} return 0; }
int FUN_0203a4ec() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203a4ec\n");} return 0; }
int FUN_0203a4fc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203a4fc\n");} return 0; }
int FUN_0203a50c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203a50c\n");} return 0; }
int FUN_0203a514() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203a514\n");} return 0; }
int FUN_0203a51c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203a51c\n");} return 0; }
int FUN_0203a738() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203a738\n");} return 0; }
int FUN_0203aa50() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203aa50\n");} return 0; }
int FUN_0203aae8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203aae8\n");} return 0; }
int FUN_0203aca0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203aca0\n");} return 0; }
int FUN_0203acb0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203acb0\n");} return 0; }
int FUN_0203acc0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203acc0\n");} return 0; }
int FUN_0203ae24() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203ae24\n");} return 0; }
int FUN_0203ae38() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203ae38\n");} return 0; }
int FUN_0203ae4c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203ae4c\n");} return 0; }
int FUN_0203ae64() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203ae64\n");} return 0; }
int FUN_0203ae78() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203ae78\n");} return 0; }
int FUN_0203ae90() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203ae90\n");} return 0; }
int FUN_0203ae9c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203ae9c\n");} return 0; }
int FUN_0203aea8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203aea8\n");} return 0; }
int FUN_0203aeb4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203aeb4\n");} return 0; }
int FUN_0203aec8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203aec8\n");} return 0; }
int FUN_0203b0d0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203b0d0\n");} return 0; }
int FUN_0203b160() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203b160\n");} return 0; }
int FUN_0203b5b4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203b5b4\n");} return 0; }
int FUN_0203b7c0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203b7c0\n");} return 0; }
int FUN_0203b7dc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203b7dc\n");} return 0; }
int FUN_0203b7f0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203b7f0\n");} return 0; }
int FUN_0203b808() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203b808\n");} return 0; }
int FUN_0203b854() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203b854\n");} return 0; }
int FUN_0203b8c4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203b8c4\n");} return 0; }
int FUN_0203b914() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203b914\n");} return 0; }
int FUN_0203b9a8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203b9a8\n");} return 0; }
int FUN_0203bad8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203bad8\n");} return 0; }
int FUN_0203bd84() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203bd84\n");} return 0; }
int FUN_0203be94() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203be94\n");} return 0; }
int FUN_0203bec8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203bec8\n");} return 0; }
int FUN_0203bef4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203bef4\n");} return 0; }
int FUN_0203bf20() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203bf20\n");} return 0; }
int FUN_0203bf4c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203bf4c\n");} return 0; }
int FUN_0203bfa4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203bfa4\n");} return 0; }
int FUN_0203bfd0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203bfd0\n");} return 0; }
int FUN_0203c004() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c004\n");} return 0; }
int FUN_0203c030() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c030\n");} return 0; }
int FUN_0203c05c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c05c\n");} return 0; }
int FUN_0203c0a0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c0a0\n");} return 0; }
int FUN_0203c0ec() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c0ec\n");} return 0; }
int FUN_0203c15c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c15c\n");} return 0; }
int FUN_0203c190() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c190\n");} return 0; }
int FUN_0203c1c4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c1c4\n");} return 0; }
int FUN_0203c1ec() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c1ec\n");} return 0; }
int FUN_0203c218() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c218\n");} return 0; }
int FUN_0203c230() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c230\n");} return 0; }
int FUN_0203c248() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c248\n");} return 0; }
int FUN_0203c274() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c274\n");} return 0; }
int FUN_0203c2a8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c2a8\n");} return 0; }
int FUN_0203c2fc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c2fc\n");} return 0; }
int FUN_0203c4b8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c4b8\n");} return 0; }
int FUN_0203c558() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c558\n");} return 0; }
int FUN_0203c5ac() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c5ac\n");} return 0; }
int FUN_0203c5ec() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c5ec\n");} return 0; }
int FUN_0203c67c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c67c\n");} return 0; }
int FUN_0203c920() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203c920\n");} return 0; }
int FUN_0203cd00() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203cd00\n");} return 0; }
int FUN_0203cd34() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203cd34\n");} return 0; }
int FUN_0203cda8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203cda8\n");} return 0; }
int FUN_0203cdf0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203cdf0\n");} return 0; }
int FUN_0203ce28() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203ce28\n");} return 0; }
int FUN_0203ce30() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203ce30\n");} return 0; }
int FUN_0203d008() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203d008\n");} return 0; }
int FUN_0203d028() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203d028\n");} return 0; }
int FUN_0203d220() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203d220\n");} return 0; }
int FUN_0203d26c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203d26c\n");} return 0; }
int FUN_0203d30c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203d30c\n");} return 0; }
int FUN_0203e328() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203e328\n");} return 0; }
int FUN_0203ed60() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203ed60\n");} return 0; }
int FUN_0203edcc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203edcc\n");} return 0; }
int FUN_0203ef08() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203ef08\n");} return 0; }
int FUN_0203ef50() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203ef50\n");} return 0; }
int FUN_0203efa0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203efa0\n");} return 0; }
int FUN_0203f2a0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203f2a0\n");} return 0; }
int FUN_0203f2c8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203f2c8\n");} return 0; }
int FUN_0203f378() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203f378\n");} return 0; }
int FUN_0203f398() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203f398\n");} return 0; }
int FUN_0203f740() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203f740\n");} return 0; }
int FUN_0203f75c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203f75c\n");} return 0; }
int FUN_0203f864() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203f864\n");} return 0; }
int FUN_0203faa8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203faa8\n");} return 0; }
int FUN_0203fb40() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203fb40\n");} return 0; }
int FUN_0203fd64() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203fd64\n");} return 0; }
int FUN_0203fd8c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203fd8c\n");} return 0; }
int FUN_0203fdd0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0203fdd0\n");} return 0; }
int FUN_020400c4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020400c4\n");} return 0; }
int FUN_020403c4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020403c4\n");} return 0; }
int FUN_020404b0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020404b0\n");} return 0; }
int FUN_0204056c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0204056c\n");} return 0; }
int FUN_020412ac() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020412ac\n");} return 0; }
int FUN_020412c4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020412c4\n");} return 0; }
int FUN_020412dc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020412dc\n");} return 0; }
int FUN_020412f4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020412f4\n");} return 0; }
int FUN_020413e4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020413e4\n");} return 0; }
int FUN_0204180c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0204180c\n");} return 0; }
int FUN_02041c60() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02041c60\n");} return 0; }
int FUN_02041d80() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02041d80\n");} return 0; }
int FUN_02041de8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02041de8\n");} return 0; }
int FUN_020425a0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020425a0\n");} return 0; }
int FUN_020425b0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020425b0\n");} return 0; }
int FUN_020425e0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020425e0\n");} return 0; }
int FUN_02042600() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02042600\n");} return 0; }
int FUN_020426ac() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020426ac\n");} return 0; }
int FUN_020426e8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020426e8\n");} return 0; }
int FUN_0204280c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0204280c\n");} return 0; }
int FUN_020428b0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020428b0\n");} return 0; }
int FUN_02042d4c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02042d4c\n");} return 0; }
int FUN_02042e1c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02042e1c\n");} return 0; }
int FUN_02042eec() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02042eec\n");} return 0; }
int FUN_020432a8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020432a8\n");} return 0; }
int FUN_0204333c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0204333c\n");} return 0; }
int FUN_02043bf4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02043bf4\n");} return 0; }
int FUN_02043ed8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02043ed8\n");} return 0; }
int FUN_02043f50() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02043f50\n");} return 0; }
int FUN_02043f70() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02043f70\n");} return 0; }
int FUN_020441e8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020441e8\n");} return 0; }
int FUN_02044644() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02044644\n");} return 0; }
int FUN_020462a8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020462a8\n");} return 0; }
int FUN_02046300() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02046300\n");} return 0; }
int FUN_020464b0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020464b0\n");} return 0; }
int FUN_020466bc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020466bc\n");} return 0; }
int FUN_02048880() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02048880\n");} return 0; }
int FUN_02048c6c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02048c6c\n");} return 0; }
int FUN_02048ce0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02048ce0\n");} return 0; }
int FUN_0206c6cc() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0206c6cc\n");} return 0; }
int FUN_0206f080() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0206f080\n");} return 0; }
int FUN_0206f600() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0206f600\n");} return 0; }
int FUN_02070a94() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02070a94\n");} return 0; }
int FUN_0207271c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_0207271c\n");} return 0; }
int FUN_020739ec() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020739ec\n");} return 0; }
int FUN_02077784() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02077784\n");} return 0; }
int FUN_020787b4() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020787b4\n");} return 0; }

/* === thunk_FUN_* stubs (3 symbols) === */
int thunk_FUN_02029ab8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] thunk_FUN_02029ab8\n");} return 0; }
int thunk_FUN_0202ed98() { static int s; if(!s){s=1; fprintf(stderr,"[stub] thunk_FUN_0202ed98\n");} return 0; }
int thunk_FUN_0203d660() { static int s; if(!s){s=1; fprintf(stderr,"[stub] thunk_FUN_0203d660\n");} return 0; }

/* === func_0x* stubs (8 symbols) === */
int func_0x01ff861c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] func_0x01ff861c\n");} return 0; }
int func_0x0209bb68() { static int s; if(!s){s=1; fprintf(stderr,"[stub] func_0x0209bb68\n");} return 0; }
int func_0x0209bb74() { static int s; if(!s){s=1; fprintf(stderr,"[stub] func_0x0209bb74\n");} return 0; }
int func_0x0209bc30() { static int s; if(!s){s=1; fprintf(stderr,"[stub] func_0x0209bc30\n");} return 0; }
int func_0x0209bc3c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] func_0x0209bc3c\n");} return 0; }
int func_0x0209c9f8() { static int s; if(!s){s=1; fprintf(stderr,"[stub] func_0x0209c9f8\n");} return 0; }
int func_0x0209ca0c() { static int s; if(!s){s=1; fprintf(stderr,"[stub] func_0x0209ca0c\n");} return 0; }
int func_0x0209cd64() { static int s; if(!s){s=1; fprintf(stderr,"[stub] func_0x0209cd64\n");} return 0; }

/* === Named SDK stubs (14 symbols) === */
int enable_ipc_interrupt() { return 0; }
int gx_alloc_render_memory() { return 0; }
int gx_disable_oam_mapping() { return 0; }
int gx_disable_vram_mapping() { return 0; }
int gx_enable_vram_mapping() { return 0; }
int gx_get_tile_state() { return 0; }
int gx_process_display_frame() { return 0; }
int gx_set_tile_state() { return 0; }
int gx_update_display_sprites() { return 0; }
int gx_update_display_state() { return 0; }
int gx_update_layer_animation() { return 0; }
int gx_verify_region_crc() { return 0; }
int ipc_reset_cmd() { return 0; }

#pragma GCC diagnostic pop

/* === Render-pipeline stubs (overlay 0/6 — added by overlay-loader session) === */
int FUN_02029518() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02029518 (RenderOam_Transfer)\n");} return 0; }
int FUN_020294b0() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_020294b0 (render finalize)\n");} return 0; }
int FUN_02065f40() { static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02065f40 (entity prep)\n");} return 0; }