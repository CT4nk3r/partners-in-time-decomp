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
uintptr_t DAT_020073bc;
uintptr_t DAT_020073c0;
uintptr_t DAT_020073c4;
uintptr_t DAT_020073c8;
uintptr_t DAT_020073cc;
uintptr_t DAT_02007478;
uintptr_t DAT_0200747c;
uintptr_t DAT_02007480;
uintptr_t DAT_02007524;
uintptr_t DAT_02007528;
uintptr_t DAT_0200752c;
uintptr_t DAT_02007584;
uintptr_t DAT_02007588;
uintptr_t DAT_0200758c;
uintptr_t DAT_02007618;
uintptr_t DAT_0200761c;
uintptr_t DAT_02007620;
uintptr_t DAT_02007624;
uintptr_t DAT_02007628;
uintptr_t DAT_02007640;
uintptr_t DAT_02007644;
uintptr_t DAT_0200780c;
uintptr_t DAT_020079a4;
uintptr_t DAT_020079a8;
uintptr_t DAT_020079ac;
uintptr_t DAT_020079b0;
uintptr_t DAT_020163ac;
uintptr_t DAT_020163b0;
uintptr_t DAT_020163b4;
uintptr_t DAT_02016570;
uintptr_t DAT_02016574;
uintptr_t DAT_02016578;
uintptr_t DAT_0201657c;
uintptr_t DAT_02016580;
uintptr_t DAT_02016584;
uintptr_t DAT_020169bc;
uintptr_t DAT_02016bcc;
uintptr_t DAT_02016bd0;
uintptr_t DAT_02017980;
uintptr_t DAT_02017984;
uintptr_t DAT_02017b10;
uintptr_t DAT_02017cfc;
uintptr_t DAT_02017dcc;
uintptr_t DAT_02018728;
uintptr_t DAT_0201872c;
uintptr_t DAT_02018730;
uintptr_t DAT_02018734;
uintptr_t DAT_02018738;
uintptr_t DAT_0201873c;
uintptr_t DAT_02018740;
uintptr_t DAT_02018744;
uintptr_t DAT_02018ab0;
uintptr_t DAT_02018ab4;
uintptr_t DAT_02018ab8;
uintptr_t DAT_02018bb8;
uintptr_t DAT_02018ca4;
uintptr_t DAT_02018d0c;
uintptr_t DAT_02018d10;
uintptr_t DAT_02018e98;
uintptr_t DAT_02018e9c;
uintptr_t DAT_02018f08;
uintptr_t DAT_02018f0c;
uintptr_t DAT_020192d4;
uintptr_t DAT_020192d8;
uintptr_t DAT_020192dc;
uintptr_t DAT_020192e0;
uintptr_t DAT_020192e4;
uintptr_t DAT_020192e8;
uintptr_t DAT_020192ec;
uintptr_t DAT_020192f0;
uintptr_t DAT_020192f4;
uintptr_t DAT_02019730;
uintptr_t DAT_02019734;
uintptr_t DAT_02019738;
uintptr_t DAT_0201973c;
uintptr_t DAT_02019740;
uintptr_t DAT_02019744;
uintptr_t DAT_02019748;
uintptr_t DAT_0201974c;
uintptr_t DAT_02019750;
uintptr_t DAT_02019754;
uintptr_t DAT_02019758;
uintptr_t DAT_0201975c;
uintptr_t DAT_02019760;
uintptr_t DAT_02019764;
uintptr_t DAT_02019768;
uintptr_t DAT_0201976c;
uintptr_t DAT_02019770;
uintptr_t DAT_02019774;
uintptr_t DAT_02019778;
uintptr_t DAT_0201977c;
uintptr_t DAT_02019780;
uintptr_t DAT_02019784;
uintptr_t DAT_02019788;
uintptr_t DAT_0201978c;
uintptr_t DAT_02019790;
uintptr_t DAT_020197bc;
uintptr_t DAT_02019b40;
uintptr_t DAT_02019e98;
uintptr_t DAT_0201a154;
uintptr_t DAT_0201a158;
uintptr_t DAT_0201a15c;
uintptr_t DAT_0201a684;
uintptr_t DAT_0201a688;
uintptr_t DAT_0201a68c;
uintptr_t DAT_0201a690;
uintptr_t DAT_0201a694;
uintptr_t DAT_0201abe0;
uintptr_t DAT_0201abe4;
uintptr_t DAT_0201abe8;
uintptr_t DAT_0201abec;
uintptr_t DAT_0201addc;
uintptr_t DAT_0201ade0;
uintptr_t DAT_0201ade4;
uintptr_t DAT_0201ade8;
uintptr_t DAT_0201adec;
uintptr_t DAT_0201adf0;
uintptr_t DAT_0201adf4;
uintptr_t DAT_0201adf8;
uintptr_t DAT_0201adfc;
uintptr_t DAT_0201ae00;
uintptr_t DAT_0201b300;
uintptr_t DAT_0201b304;
uintptr_t DAT_0201b308;
uintptr_t DAT_0201b30c;
uintptr_t DAT_0201b408;
uintptr_t DAT_0201b40c;
uintptr_t DAT_0201b410;
uintptr_t DAT_0201b414;
uintptr_t DAT_0201c6f4;
uintptr_t DAT_0201d0bc;
uintptr_t DAT_0201d0c0;
uintptr_t DAT_0201d0c4;
uintptr_t DAT_0201d0c8;
uintptr_t DAT_0201d314;
uintptr_t DAT_0201d9b4;
uintptr_t DAT_0201d9b8;
uintptr_t DAT_0201d9bc;
uintptr_t DAT_0201d9c0;
uintptr_t DAT_0201dcc8;
uintptr_t DAT_0201dccc;
uintptr_t DAT_0201dcd0;
uintptr_t DAT_0201dcd4;
uintptr_t DAT_0201e820;
uintptr_t DAT_0201e824;
uintptr_t DAT_0201e828;
uintptr_t DAT_0201e82c;
uintptr_t DAT_0201e830;
uintptr_t DAT_0201e834;
uintptr_t DAT_0201eedc;
uintptr_t DAT_0201eee0;
uintptr_t DAT_0201eee4;
uintptr_t DAT_0201eee8;
uintptr_t DAT_0201eeec;
uintptr_t DAT_0201eef0;
uintptr_t DAT_0201eef4;
uintptr_t DAT_0201f04c;
uintptr_t DAT_0201f1e0;
uintptr_t DAT_0201f1e4;
uintptr_t DAT_0201f1e8;
uintptr_t DAT_0201f41c;
uintptr_t DAT_0201f4e4;
uintptr_t DAT_0201f5a4;
uintptr_t DAT_0201fa10;
uintptr_t DAT_0201fa14;
uintptr_t DAT_0201fa18;
uintptr_t DAT_0201fa1c;
uintptr_t DAT_0201fa20;
uintptr_t DAT_0201fa24;
uintptr_t DAT_0201fa28;
uintptr_t DAT_0201fa2c;
uintptr_t DAT_0201fa30;
uintptr_t DAT_0201fa34;
uintptr_t DAT_0201fa38;
uintptr_t DAT_0201fa3c;
uintptr_t DAT_0201fa40;
uintptr_t DAT_0201fa44;
uintptr_t DAT_0201fa48;
uintptr_t DAT_0201fa4c;
uintptr_t DAT_0201feb8;
uintptr_t DAT_0201febc;
uintptr_t DAT_0201fec0;
uintptr_t DAT_0201fec4;
uintptr_t DAT_0201fec8;
uintptr_t DAT_0201fecc;
uintptr_t DAT_0201fed0;
uintptr_t DAT_0201fed4;
uintptr_t DAT_0201fed8;
uintptr_t DAT_0201fedc;
uintptr_t DAT_0201fee0;
uintptr_t DAT_0201fee4;
uintptr_t DAT_0201fee8;
uintptr_t DAT_0201feec;
uintptr_t DAT_0201fef0;
uintptr_t DAT_0201fef4;
uintptr_t DAT_02020290;
uintptr_t DAT_02020294;
uintptr_t DAT_020204b0;
uintptr_t DAT_020204b4;
uintptr_t DAT_020204b8;
uintptr_t DAT_0202055c;
uintptr_t DAT_02020858;
uintptr_t DAT_0202085c;
uintptr_t DAT_02020aac;
uintptr_t DAT_02020ab0;
uintptr_t DAT_02020ab4;
uintptr_t DAT_02020da0;
uintptr_t DAT_02020da4;
uintptr_t DAT_02020da8;
uintptr_t DAT_02020dac;
uintptr_t DAT_02020db0;
uintptr_t DAT_02020db4;
uintptr_t DAT_02020f1c;
uintptr_t DAT_02020f54;
uintptr_t DAT_02020fe4;
uintptr_t DAT_02021124;
uintptr_t DAT_02021128;
uintptr_t DAT_02021578;
uintptr_t DAT_0202157c;
uintptr_t DAT_02021580;
uintptr_t DAT_02021584;
uintptr_t DAT_020216e4;
uintptr_t DAT_020216e8;
uintptr_t DAT_020216ec;
uintptr_t DAT_02021acc;
uintptr_t DAT_02021ad0;
uintptr_t DAT_02021ad4;
uintptr_t DAT_02021ad8;
uintptr_t DAT_02021c2c;
uintptr_t DAT_02021c30;
uintptr_t DAT_02021f54;
uintptr_t DAT_02021f58;
uintptr_t DAT_02021f5c;
uintptr_t DAT_02021f60;
uintptr_t DAT_02021f64;
uintptr_t DAT_02021f68;
uintptr_t DAT_02021f6c;
uintptr_t DAT_02021f70;
uintptr_t DAT_02021f74;
uintptr_t DAT_02021f78;
uintptr_t DAT_02024a98;
uintptr_t DAT_02024a9c;
uintptr_t DAT_02024aa0;
uintptr_t DAT_02024aa4;
uintptr_t DAT_02024aa8;
uintptr_t DAT_02025184;
uintptr_t DAT_02025188;
uintptr_t DAT_0202518c;
uintptr_t DAT_02025190;
uintptr_t DAT_020257fc;
uintptr_t DAT_02025800;
uintptr_t DAT_02025804;
uintptr_t DAT_02025808;
uintptr_t DAT_0202580c;
uintptr_t DAT_020265a8;
uintptr_t DAT_02026794;
uintptr_t DAT_02026798;
uintptr_t DAT_0202679c;
uintptr_t DAT_020267a0;
uintptr_t DAT_020267a4;
uintptr_t DAT_020267a8;
uintptr_t DAT_020267ac;
uintptr_t DAT_020267b0;
uintptr_t DAT_020267b4;
uintptr_t DAT_020267b8;
uintptr_t DAT_020267bc;
uintptr_t DAT_020267c0;
uintptr_t DAT_020267c4;
uintptr_t DAT_020267c8;
uintptr_t DAT_020267cc;
uintptr_t DAT_020267d0;
uintptr_t DAT_020267d4;
uintptr_t DAT_020267d8;
uintptr_t DAT_020267dc;
uintptr_t DAT_020267e0;
uintptr_t DAT_020267e4;
uintptr_t DAT_020267e8;
uintptr_t DAT_020267ec;
uintptr_t DAT_020267f0;
uintptr_t DAT_020267f4;
uintptr_t DAT_020267f8;
uintptr_t DAT_020267fc;
uintptr_t DAT_02026800;
uintptr_t DAT_02026804;
uintptr_t DAT_02026808;
uintptr_t DAT_0202680c;
uintptr_t DAT_02026810;
uintptr_t DAT_02026814;
uintptr_t DAT_02026818;
uintptr_t DAT_0202681c;
uintptr_t DAT_02026820;
uintptr_t DAT_02026824;
uintptr_t DAT_02026828;
uintptr_t DAT_0202682c;
uintptr_t DAT_02026860;
uintptr_t DAT_02026864;
uintptr_t DAT_02026b10;
uintptr_t DAT_02026b14;
uintptr_t DAT_02026be8;
uintptr_t DAT_02026c68;
uintptr_t DAT_02026c6c;
uintptr_t DAT_02026ca8;
uintptr_t DAT_02026cc8;
uintptr_t DAT_02026ccc;
uintptr_t DAT_02026e10;
uintptr_t DAT_02027014;
uintptr_t DAT_02027018;
uintptr_t DAT_0202701c;
uintptr_t DAT_020270c4;
uintptr_t DAT_020270c8;
uintptr_t DAT_020270cc;
uintptr_t DAT_020270d0;
uintptr_t DAT_0202728c;
uintptr_t DAT_02027290;
uintptr_t DAT_02027294;
uintptr_t DAT_02027298;
uintptr_t DAT_0202729c;
uintptr_t DAT_02027368;
uintptr_t DAT_0202736c;
uintptr_t DAT_02027370;
uintptr_t DAT_02027374;
uintptr_t DAT_02027378;
uintptr_t DAT_0202737c;
uintptr_t DAT_020273f8;
uintptr_t DAT_020273fc;
uintptr_t DAT_02027400;
uintptr_t DAT_020274fc;
uintptr_t DAT_02027500;
uintptr_t DAT_02027504;
uintptr_t DAT_02027508;
uintptr_t DAT_0202750c;
uintptr_t DAT_02027510;
uintptr_t DAT_020275d4;
uintptr_t DAT_020275d8;
uintptr_t DAT_020275dc;
uintptr_t DAT_020275e0;
uintptr_t DAT_0202764c;
uintptr_t DAT_02027650;
uintptr_t DAT_0202769c;
uintptr_t DAT_0202770c;
uintptr_t DAT_02027844;
uintptr_t DAT_02027848;
uintptr_t DAT_0202784c;
uintptr_t DAT_02027850;
uintptr_t DAT_020278f4;
uintptr_t DAT_020278f8;
uintptr_t DAT_02027a58;
uintptr_t DAT_02027a5c;
uintptr_t DAT_02027a60;
uintptr_t DAT_02027a64;
uintptr_t DAT_02027b28;
uintptr_t DAT_02027b2c;
uintptr_t DAT_02027b30;
uintptr_t DAT_02027b34;
uintptr_t DAT_02027c70;
uintptr_t DAT_02027c74;
uintptr_t DAT_02027cec;
uintptr_t DAT_02027cf0;
uintptr_t DAT_02027edc;
uintptr_t DAT_02027fe8;
uintptr_t DAT_02027fec;
uintptr_t DAT_02027ff0;
uintptr_t DAT_02028694;
uintptr_t DAT_02028698;
uintptr_t DAT_0202869c;
uintptr_t DAT_020286a0;
uintptr_t DAT_020286a4;
uintptr_t DAT_020286a8;
uintptr_t DAT_020286ac;
uintptr_t DAT_020289e4;
uintptr_t DAT_020289e8;
uintptr_t DAT_020289ec;
uintptr_t DAT_020289f0;
uintptr_t DAT_02028aac;
uintptr_t DAT_02028ab0;
uintptr_t DAT_02028b00;
uintptr_t DAT_02028b48;
uintptr_t DAT_02028b4c;
uintptr_t DAT_02028bcc;
uintptr_t DAT_02028c1c;
uintptr_t DAT_02028cd4;
uintptr_t DAT_02028cd8;
uintptr_t DAT_02028e98;
uintptr_t DAT_02028e9c;
uintptr_t DAT_02028ea0;
uintptr_t DAT_02028ea4;
uintptr_t DAT_020293b0;
uintptr_t DAT_020293b4;
uintptr_t DAT_020293b8;
uintptr_t DAT_020293bc;
uintptr_t DAT_020293c0;
uintptr_t DAT_020293c4;
uintptr_t DAT_02029478;
uintptr_t DAT_0202947c;
uintptr_t DAT_02029480;
uintptr_t DAT_02029484;
uintptr_t DAT_02029720;
uintptr_t DAT_02029758;
uintptr_t DAT_0202975c;
uintptr_t DAT_02029760;
uintptr_t DAT_02029764;
uintptr_t DAT_02029768;
uintptr_t DAT_02029838;
uintptr_t DAT_0202983c;
uintptr_t DAT_02029840;
uintptr_t DAT_02029958;
uintptr_t DAT_0202995c;
uintptr_t DAT_02029960;
uintptr_t DAT_02029df0;
uintptr_t DAT_02029df4;
uintptr_t DAT_02029df8;
uintptr_t DAT_02029dfc;
uintptr_t DAT_02029e00;
uintptr_t DAT_02029e04;
uintptr_t DAT_02029e08;
uintptr_t DAT_02029e0c;
uintptr_t DAT_02029e10;
uintptr_t DAT_02029e14;
uintptr_t DAT_02029e18;
uintptr_t DAT_02029e1c;
uintptr_t DAT_02029f84;
uintptr_t DAT_02029f88;
uintptr_t DAT_02029f8c;
uintptr_t DAT_02029ff8;
uintptr_t DAT_0202a174;
uintptr_t DAT_0202a178;
uintptr_t DAT_0202a17c;
uintptr_t DAT_0202a180;
uintptr_t DAT_0202a184;
uintptr_t DAT_0202a510;
uintptr_t DAT_0202a514;
uintptr_t DAT_0202a518;
uintptr_t DAT_0202a568;
uintptr_t DAT_0202a6b4;
uintptr_t DAT_0202a6b8;
uintptr_t DAT_0202a6bc;
uintptr_t DAT_0202a6c0;
uintptr_t DAT_0202a6c4;
uintptr_t DAT_0202a9f0;
uintptr_t DAT_0202aaac;
uintptr_t DAT_0202ac2c;
uintptr_t DAT_0202af10;
uintptr_t DAT_0202b1dc;
uintptr_t DAT_0202b1e0;
uintptr_t DAT_0202b1e4;
uintptr_t DAT_0202b1e8;
uintptr_t DAT_0202b1ec;
uintptr_t DAT_0202b1f0;
uintptr_t DAT_0202b1f4;
uintptr_t DAT_0202b1f8;
uintptr_t DAT_0202b1fc;
uintptr_t DAT_0202b200;
uintptr_t DAT_0202b204;
uintptr_t DAT_0202b6ac;
uintptr_t DAT_0202b6c0;
uintptr_t DAT_0202b71c;
uintptr_t DAT_0202b898;
uintptr_t DAT_0202b89c;
uintptr_t DAT_0202b920;
uintptr_t DAT_0202b924;
uintptr_t DAT_0202b928;
uintptr_t DAT_0202b940;
uintptr_t DAT_0202bcb0;
uintptr_t DAT_0202bcb4;
uintptr_t DAT_0202bcfc;
uintptr_t DAT_0202bd4c;
uintptr_t DAT_0202bd94;
uintptr_t DAT_0202bea0;
uintptr_t DAT_0202bea4;
uintptr_t DAT_0202bff4;
uintptr_t DAT_0202c0cc;
uintptr_t DAT_0202c100;
uintptr_t DAT_0202c104;
uintptr_t DAT_0202c108;
uintptr_t DAT_0202c77c;
uintptr_t DAT_0202c780;
uintptr_t DAT_0202c7d0;
uintptr_t DAT_0202c828;
uintptr_t DAT_0202c85c;
uintptr_t DAT_0202c904;
uintptr_t DAT_0202c9bc;
uintptr_t DAT_0202cadc;
uintptr_t DAT_0202cae0;
uintptr_t DAT_0202cb0c;
uintptr_t DAT_0202cb80;
uintptr_t DAT_0202d158;
uintptr_t DAT_0202dce0;
uintptr_t DAT_0202dec0;
uintptr_t DAT_0202dffc;
uintptr_t DAT_0202e000;
uintptr_t DAT_0202e004;
uintptr_t DAT_0202e008;
uintptr_t DAT_0202e00c;
uintptr_t DAT_0202e01c;
uintptr_t DAT_0202e070;
uintptr_t DAT_0202e074;
uintptr_t DAT_0202e084;
uintptr_t DAT_0202e0d0;
uintptr_t DAT_0202e0d4;
uintptr_t DAT_0202e25c;
uintptr_t DAT_0202e260;
uintptr_t DAT_0202e264;
uintptr_t DAT_0202e268;
uintptr_t DAT_0202e26c;
uintptr_t DAT_0202e270;
uintptr_t DAT_0202e304;
uintptr_t DAT_0202e308;
uintptr_t DAT_0202e30c;
uintptr_t DAT_0202e310;
uintptr_t DAT_0202e320;
uintptr_t DAT_0202e354;
uintptr_t DAT_0202e358;
uintptr_t DAT_0202e3b8;
uintptr_t DAT_0202e3bc;
uintptr_t DAT_0202e68c;
uintptr_t DAT_0202e690;
uintptr_t DAT_0202e6bc;
uintptr_t DAT_0202ea54;
uintptr_t DAT_0202eb60;
uintptr_t DAT_0202eb64;
uintptr_t DAT_0202eb68;
uintptr_t DAT_0202eb6c;
uintptr_t DAT_0202eb70;
uintptr_t DAT_0202eb74;
uintptr_t DAT_0202eb78;
uintptr_t DAT_0202eb9c;
uintptr_t DAT_0202eba0;
uintptr_t DAT_0202eba4;
uintptr_t DAT_0202ebc4;
uintptr_t DAT_0202ec24;
uintptr_t DAT_0202ec48;
uintptr_t DAT_0202ec80;
uintptr_t DAT_0202ecd4;
uintptr_t DAT_0202ed38;
uintptr_t DAT_0202ed94;
uintptr_t DAT_0202ee20;
uintptr_t DAT_0202ee24;
uintptr_t DAT_0202ee9c;
uintptr_t DAT_0202eea0;
uintptr_t DAT_0202eedc;
uintptr_t DAT_0202ef3c;
uintptr_t DAT_0202f040;
uintptr_t DAT_0202f0fc;
uintptr_t DAT_0202f1c4;
uintptr_t DAT_0202f330;
uintptr_t DAT_0202f334;
uintptr_t DAT_0202f338;
uintptr_t DAT_0202f3e4;
uintptr_t DAT_0202f3e8;
uintptr_t DAT_0202f3ec;
uintptr_t DAT_0202f3f0;
uintptr_t DAT_0202f490;
uintptr_t DAT_0202f58c;
uintptr_t DAT_0202f620;
uintptr_t DAT_0202f688;
uintptr_t DAT_0202f698;
uintptr_t DAT_0202f734;
uintptr_t DAT_0202f738;
uintptr_t DAT_0202f770;
uintptr_t DAT_0202f8f0;
uintptr_t DAT_0202f8f4;
uintptr_t DAT_0202f938;
uintptr_t DAT_0202fa70;
uintptr_t DAT_0202fcc4;
uintptr_t DAT_0202fcc8;
uintptr_t DAT_0202fccc;
uintptr_t DAT_0202fdd8;
uintptr_t DAT_0202fddc;
uintptr_t DAT_0202fde0;
uintptr_t DAT_0202fde4;
uintptr_t DAT_0202fe50;
uintptr_t DAT_0202ff48;
uintptr_t DAT_0202ff4c;
uintptr_t DAT_0202ff50;
uintptr_t DAT_0202ff54;
uintptr_t DAT_02030054;
uintptr_t DAT_0203014c;
uintptr_t DAT_02030150;
uintptr_t DAT_020304b0;
uintptr_t DAT_020304b4;
uintptr_t DAT_020304b8;
uintptr_t DAT_0203060c;
uintptr_t DAT_02030610;
uintptr_t DAT_02030614;
uintptr_t DAT_02030618;
uintptr_t DAT_0203061c;
uintptr_t DAT_02030620;
uintptr_t DAT_02030624;
uintptr_t DAT_0203066c;
uintptr_t DAT_0203072c;
uintptr_t DAT_020308d0;
uintptr_t DAT_020309a4;
uintptr_t DAT_02030a08;
uintptr_t DAT_02030a6c;
uintptr_t DAT_02030ad0;
uintptr_t DAT_02030b34;
uintptr_t DAT_02030c60;
uintptr_t DAT_02030cd4;
uintptr_t DAT_02030cec;
uintptr_t DAT_02030f00;
uintptr_t DAT_02030f04;
uintptr_t DAT_02030f08;
uintptr_t DAT_02030f90;
uintptr_t DAT_02031054;
uintptr_t DAT_02031550;
uintptr_t DAT_02031810;
uintptr_t DAT_02031814;
uintptr_t DAT_02031890;
uintptr_t DAT_0203190c;
uintptr_t DAT_02031988;
uintptr_t DAT_02031a04;
uintptr_t DAT_02031cc0;
uintptr_t DAT_02032380;
uintptr_t DAT_02032384;
uintptr_t DAT_02032388;
uintptr_t DAT_02032c2c;
uintptr_t DAT_02032c30;
uintptr_t DAT_02032c34;
uintptr_t DAT_02032c38;
uintptr_t DAT_02032c3c;
uintptr_t DAT_02032d88;
uintptr_t DAT_02032ecc;
uintptr_t DAT_02032ed0;
uintptr_t DAT_02032ed4;
uintptr_t DAT_02032ed8;
uintptr_t DAT_02032edc;
uintptr_t DAT_02032f80;
uintptr_t DAT_02032f84;
uintptr_t DAT_02032f88;
uintptr_t DAT_02032fc0;
uintptr_t DAT_0203300c;
uintptr_t DAT_02033140;
uintptr_t DAT_02033144;
uintptr_t DAT_02033148;
uintptr_t DAT_0203314c;
uintptr_t DAT_0203323c;
uintptr_t DAT_02033240;
uintptr_t DAT_02033244;
uintptr_t DAT_020332dc;
uintptr_t DAT_020332e0;
uintptr_t DAT_020332e4;
uintptr_t DAT_02033618;
uintptr_t DAT_0203361c;
uintptr_t DAT_02033704;
uintptr_t DAT_020339f4;
uintptr_t DAT_020339f8;
uintptr_t DAT_02033ae0;
uintptr_t DAT_02033ae4;
uintptr_t DAT_02033ae8;
uintptr_t DAT_02033aec;
uintptr_t DAT_02033af0;
uintptr_t DAT_02033af4;
uintptr_t DAT_02033af8;
uintptr_t DAT_02033afc;
uintptr_t DAT_02033d48;
uintptr_t DAT_02033eec;
uintptr_t DAT_020347f0;
uintptr_t DAT_020347f4;
uintptr_t DAT_020347f8;
uintptr_t DAT_020347fc;
uintptr_t DAT_0203482c;
uintptr_t DAT_02034830;
uintptr_t DAT_02034834;
uintptr_t DAT_0203485c;
uintptr_t DAT_02034860;
uintptr_t DAT_020348ac;
uintptr_t DAT_020348b0;
uintptr_t DAT_020348e4;
uintptr_t DAT_020348e8;
uintptr_t DAT_020348ec;
uintptr_t DAT_02034924;
uintptr_t DAT_02034928;
uintptr_t DAT_0203494c;
uintptr_t DAT_02034950;
uintptr_t DAT_020349c0;
uintptr_t DAT_020349c4;
uintptr_t DAT_02034a10;
uintptr_t DAT_02034a14;
uintptr_t DAT_02034b74;
uintptr_t DAT_02034b78;
uintptr_t DAT_02034b7c;
uintptr_t DAT_02034b80;
uintptr_t DAT_02034b84;
uintptr_t DAT_02034b88;
uintptr_t DAT_02034b8c;
uintptr_t DAT_02034e2c;
uintptr_t DAT_02034e30;
uintptr_t DAT_02034e34;
uintptr_t DAT_02034e38;
uintptr_t DAT_02034e3c;
uintptr_t DAT_02034e40;
uintptr_t DAT_02034e44;
uintptr_t DAT_02034e48;
uintptr_t DAT_0203501c;
uintptr_t DAT_02035108;
uintptr_t DAT_02035210;
uintptr_t DAT_02035214;
uintptr_t DAT_02035258;
uintptr_t DAT_0203525c;
uintptr_t DAT_020355c0;
uintptr_t DAT_020355d0;
uintptr_t DAT_0203580c;
uintptr_t DAT_02035810;
uintptr_t DAT_02035814;
uintptr_t DAT_02035834;
uintptr_t DAT_02035838;
uintptr_t DAT_0203583c;
uintptr_t DAT_0203584c;
uintptr_t DAT_02035850;
uintptr_t DAT_02035860;
uintptr_t DAT_02035864;
uintptr_t DAT_02035884;
uintptr_t DAT_02035888;
uintptr_t DAT_020358a8;
uintptr_t DAT_020358ac;
uintptr_t DAT_02035968;
uintptr_t DAT_0203596c;
uintptr_t DAT_02035970;
uintptr_t DAT_020359f4;
uintptr_t DAT_020359f8;
uintptr_t DAT_020359fc;
uintptr_t DAT_02035a70;
uintptr_t DAT_02035a74;
uintptr_t DAT_02035a78;
uintptr_t DAT_02035b18;
uintptr_t DAT_02035b1c;
uintptr_t DAT_02035b20;
uintptr_t DAT_02035b24;
uintptr_t DAT_02035c2c;
uintptr_t DAT_02035c30;
uintptr_t DAT_02035c34;
uintptr_t DAT_02035c38;
uintptr_t DAT_02035e24;
uintptr_t DAT_02035e28;
uintptr_t DAT_02035e2c;
uintptr_t DAT_02035e30;
uintptr_t DAT_02035e34;
uintptr_t DAT_02035e38;
uintptr_t DAT_02035e3c;
uintptr_t DAT_02035eec;
uintptr_t DAT_02035ef0;
uintptr_t DAT_02035ef4;
uintptr_t DAT_02035ffc;
uintptr_t DAT_02036000;
uintptr_t DAT_02036004;
uintptr_t DAT_02036008;
uintptr_t DAT_02036160;
uintptr_t DAT_02036164;
uintptr_t DAT_02036168;
uintptr_t DAT_0203616c;
uintptr_t DAT_02036170;
uintptr_t DAT_02036174;
uintptr_t DAT_02036410;
uintptr_t DAT_02036414;
uintptr_t DAT_02036418;
uintptr_t DAT_0203641c;
uintptr_t DAT_02036420;
uintptr_t DAT_02036424;
uintptr_t DAT_02036428;
uintptr_t DAT_0203642c;
uintptr_t DAT_020364c4;
uintptr_t DAT_020364c8;
uintptr_t DAT_020364cc;
uintptr_t DAT_020364d0;
uintptr_t DAT_020364d4;
uintptr_t DAT_020364d8;
uintptr_t DAT_020364dc;
uintptr_t DAT_020364e0;
uintptr_t DAT_020364e4;
uintptr_t DAT_02036594;
uintptr_t DAT_02036634;
uintptr_t DAT_02036654;
uintptr_t DAT_020366a8;
uintptr_t DAT_02036758;
uintptr_t DAT_0203675c;
uintptr_t DAT_020367e8;
uintptr_t DAT_02036864;
uintptr_t DAT_02036868;
uintptr_t DAT_020368f4;
uintptr_t DAT_02036bf0;
uintptr_t DAT_02036c1c;
uintptr_t DAT_02036cd8;
uintptr_t DAT_02036cdc;
uintptr_t DAT_02036d10;
uintptr_t DAT_02036d14;
uintptr_t DAT_020374f4;
uintptr_t DAT_020374f8;
uintptr_t DAT_020377e8;
uintptr_t DAT_020377ec;
uintptr_t DAT_020377f0;
uintptr_t DAT_020377f4;
uintptr_t DAT_020379d8;
uintptr_t DAT_020379dc;
uintptr_t DAT_020379e0;
uintptr_t DAT_020379e4;
uintptr_t DAT_02037a2c;
uintptr_t DAT_02037a30;
uintptr_t DAT_02037aa0;
uintptr_t DAT_02037aa4;
uintptr_t DAT_02037ac8;
uintptr_t DAT_02037bfc;
uintptr_t DAT_02037c00;
uintptr_t DAT_02037c04;
uintptr_t DAT_02037c74;
uintptr_t DAT_02037c78;
uintptr_t DAT_02037cdc;
uintptr_t DAT_02037ce0;
uintptr_t DAT_02037ce4;
uintptr_t DAT_02037ce8;
uintptr_t DAT_020385b0;
uintptr_t DAT_02038610;
uintptr_t DAT_02038724;
uintptr_t DAT_02038728;
uintptr_t DAT_0203872c;
uintptr_t DAT_0203879c;
uintptr_t DAT_020387a0;
uintptr_t DAT_020387e0;
uintptr_t DAT_020387e4;
uintptr_t DAT_020387e8;
uintptr_t DAT_02038848;
uintptr_t DAT_0203884c;
uintptr_t DAT_02038850;
uintptr_t DAT_02038854;
uintptr_t DAT_02038858;
uintptr_t DAT_020389b0;
uintptr_t DAT_020389b4;
uintptr_t DAT_020389b8;
uintptr_t DAT_020389bc;
uintptr_t DAT_02038a24;
uintptr_t DAT_02038a28;
uintptr_t DAT_02038a2c;
uintptr_t DAT_02038a30;
uintptr_t DAT_02038a34;
uintptr_t DAT_02038a38;
uintptr_t DAT_02038a3c;
uintptr_t DAT_02038e54;
uintptr_t DAT_02038e58;
uintptr_t DAT_02038e6c;
uintptr_t DAT_02038f10;
uintptr_t DAT_02038fc8;
uintptr_t DAT_02038fcc;
uintptr_t DAT_02038fd0;
uintptr_t DAT_020390a0;
uintptr_t DAT_02039140;
uintptr_t DAT_02039274;
uintptr_t DAT_02039278;
uintptr_t DAT_0203927c;
uintptr_t DAT_02039280;
uintptr_t DAT_02039284;
uintptr_t DAT_02039288;
uintptr_t DAT_0203938c;
uintptr_t DAT_02039390;
uintptr_t DAT_02039464;
uintptr_t DAT_02039468;
uintptr_t DAT_0203946c;
uintptr_t DAT_02039470;
uintptr_t DAT_02039544;
uintptr_t DAT_020395ac;
uintptr_t DAT_02039604;
uintptr_t DAT_02039608;
uintptr_t DAT_02039704;
uintptr_t DAT_02039708;
uintptr_t DAT_0203970c;
uintptr_t DAT_02039710;
uintptr_t DAT_02039858;
uintptr_t DAT_0203985c;
uintptr_t DAT_02039860;
uintptr_t DAT_02039864;
uintptr_t DAT_02039868;
uintptr_t DAT_0203986c;
uintptr_t DAT_02039870;
uintptr_t DAT_02039874;
uintptr_t DAT_02039878;
uintptr_t DAT_0203987c;
uintptr_t DAT_02039880;
uintptr_t DAT_02039884;
uintptr_t DAT_02039888;
uintptr_t DAT_0203988c;
uintptr_t DAT_02039890;
uintptr_t DAT_020398d8;
uintptr_t DAT_02039940;
uintptr_t DAT_02039a90;
uintptr_t DAT_02039a94;
uintptr_t DAT_02039a98;
uintptr_t DAT_02039b60;
uintptr_t DAT_02039b64;
uintptr_t DAT_02039b68;
uintptr_t DAT_02039b6c;
uintptr_t DAT_02039be0;
uintptr_t DAT_02039be4;
uintptr_t DAT_02039e44;
uintptr_t DAT_02039f04;
uintptr_t DAT_02039f90;
uintptr_t DAT_0203a1e8;
uintptr_t DAT_0203a1ec;
uintptr_t DAT_0203a1f0;
uintptr_t DAT_0203a1f4;
uintptr_t DAT_0203a1f8;
uintptr_t DAT_0203a1fc;
uintptr_t DAT_0203a30c;
uintptr_t DAT_0203a310;
uintptr_t DAT_0203a314;
uintptr_t DAT_0203a318;
uintptr_t DAT_0203a31c;
uintptr_t DAT_0203a320;
uintptr_t DAT_0203a324;
uintptr_t DAT_0203a328;
uintptr_t DAT_0203a3b0;
uintptr_t DAT_0203a3b4;
uintptr_t DAT_0203a3b8;
uintptr_t DAT_0203a3bc;
uintptr_t DAT_0203a4d4;
uintptr_t DAT_0203a698;
uintptr_t DAT_0203a69c;
uintptr_t DAT_0203a6a0;
uintptr_t DAT_0203a6a4;
uintptr_t DAT_0203a724;
uintptr_t DAT_0203a728;
uintptr_t DAT_0203a72c;
uintptr_t DAT_0203a730;
uintptr_t DAT_0203a734;
uintptr_t DAT_0203a7f4;
uintptr_t DAT_0203a7f8;
uintptr_t DAT_0203a7fc;
uintptr_t DAT_0203a800;
uintptr_t DAT_0203a878;
uintptr_t DAT_0203a87c;
uintptr_t DAT_0203a880;
uintptr_t DAT_0203a884;
uintptr_t DAT_0203a888;
uintptr_t DAT_0203a910;
uintptr_t DAT_0203a914;
uintptr_t DAT_0203a918;
uintptr_t DAT_0203a91c;
uintptr_t DAT_0203a920;
uintptr_t DAT_0203a924;
uintptr_t DAT_0203aa44;
uintptr_t DAT_0203aa48;
uintptr_t DAT_0203aa4c;
uintptr_t DAT_0203ac9c;
uintptr_t DAT_0203ada4;
uintptr_t DAT_0203ada8;
uintptr_t DAT_0203adac;
uintptr_t DAT_0203adb0;
uintptr_t DAT_0203ae14;
uintptr_t DAT_0203ae18;
uintptr_t DAT_0203ae1c;
uintptr_t DAT_0203ae20;
uintptr_t DAT_0203b01c;
uintptr_t DAT_0203b020;
uintptr_t DAT_0203b0b0;
uintptr_t DAT_0203b0c8;
uintptr_t DAT_0203b0cc;
uintptr_t DAT_0203b154;
uintptr_t DAT_0203b158;
uintptr_t DAT_0203b15c;
uintptr_t DAT_0203b2c4;
uintptr_t DAT_0203b344;
uintptr_t DAT_0203b348;
uintptr_t DAT_0203b3b4;
uintptr_t DAT_0203b3b8;
uintptr_t DAT_0203b538;
uintptr_t DAT_0203b5b0;
uintptr_t DAT_0203b6b8;
uintptr_t DAT_0203b6bc;
uintptr_t DAT_0203b730;
uintptr_t DAT_0203b7a4;
uintptr_t DAT_0203bd7c;
uintptr_t DAT_0203bd80;
uintptr_t DAT_0203c38c;
uintptr_t DAT_0203c3e0;
uintptr_t DAT_0203c3e4;
uintptr_t DAT_0203c484;
uintptr_t DAT_0203c514;
uintptr_t DAT_0203c554;
uintptr_t DAT_0203c87c;
uintptr_t DAT_0203c880;
uintptr_t DAT_0203cb34;
uintptr_t DAT_0203cb38;
uintptr_t DAT_0203cb3c;
uintptr_t DAT_0203cb40;
uintptr_t DAT_0203cb44;
uintptr_t DAT_0203cb48;
uintptr_t DAT_0203cb4c;
uintptr_t DAT_0203cb50;
uintptr_t DAT_0203cb54;
uintptr_t DAT_0203cb58;
uintptr_t DAT_0203cb5c;
uintptr_t DAT_0203cb60;
uintptr_t DAT_0203cb64;
uintptr_t DAT_0203cb68;
uintptr_t DAT_0203cbc8;
uintptr_t DAT_0203cbf8;
uintptr_t DAT_0203cc48;
uintptr_t DAT_0203ccfc;
uintptr_t DAT_0203d50c;
uintptr_t DAT_0203d510;
uintptr_t DAT_0203d514;
uintptr_t DAT_0203d5c4;
uintptr_t DAT_0203d5c8;
uintptr_t DAT_0203d5f0;
uintptr_t DAT_0203d658;
uintptr_t DAT_0203d65c;
uintptr_t DAT_0203d750;
uintptr_t DAT_0203d754;
uintptr_t DAT_0203d758;
uintptr_t DAT_0203d75c;
uintptr_t DAT_0203d760;
uintptr_t DAT_0203d764;
uintptr_t DAT_0203d768;
uintptr_t DAT_0203dbd4;
uintptr_t DAT_0203df00;
uintptr_t DAT_0203df98;
uintptr_t DAT_0203e0a0;
uintptr_t DAT_0203e2e0;
uintptr_t DAT_0203e59c;
uintptr_t DAT_0203e610;
uintptr_t DAT_0203e614;
uintptr_t DAT_0203e6d0;
uintptr_t DAT_0203e6d4;
uintptr_t DAT_0203ec80;
uintptr_t DAT_0203f29c;
uintptr_t DAT_0203f550;
uintptr_t DAT_0203f554;
uintptr_t DAT_0203f558;
uintptr_t DAT_0203f55c;
uintptr_t DAT_0203f560;
uintptr_t DAT_0203f564;
uintptr_t DAT_0203f568;
uintptr_t DAT_0203f56c;
uintptr_t DAT_0203f578;
uintptr_t DAT_0203f57c;
uintptr_t DAT_0203f580;
uintptr_t DAT_0203f584;
uintptr_t DAT_0203f628;
uintptr_t DAT_0203f678;
uintptr_t DAT_0203f67c;
uintptr_t DAT_0203fa00;
uintptr_t DAT_0203fa04;
uintptr_t DAT_0204062c;
uintptr_t DAT_02040630;
uintptr_t DAT_02040634;
uintptr_t DAT_02040638;
uintptr_t DAT_0204063c;
uintptr_t DAT_020406a0;
uintptr_t DAT_020406a4;
uintptr_t DAT_020406a8;
uintptr_t DAT_020406ac;
uintptr_t DAT_020406b0;
uintptr_t DAT_02040808;
uintptr_t DAT_0204080c;
uintptr_t DAT_02040a70;
uintptr_t DAT_02040a74;
uintptr_t DAT_02040a78;
uintptr_t DAT_02040a7c;
uintptr_t DAT_02040cc4;
uintptr_t DAT_02040d64;
uintptr_t DAT_02040e9c;
uintptr_t DAT_02040ea0;
uintptr_t DAT_02040ea4;
uintptr_t DAT_02040ea8;
uintptr_t DAT_02040eac;
uintptr_t DAT_02040f58;
uintptr_t DAT_02040fe0;
uintptr_t DAT_02040fe4;
uintptr_t DAT_02040fe8;
uintptr_t DAT_020412a0;
uintptr_t DAT_020412a4;
uintptr_t DAT_020412a8;
uintptr_t DAT_020414e4;
uintptr_t DAT_020414e8;
uintptr_t DAT_020414ec;
uintptr_t DAT_02041704;
uintptr_t DAT_02041708;
uintptr_t DAT_0204170c;
uintptr_t DAT_02041710;
uintptr_t DAT_02041714;
uintptr_t DAT_02041718;
uintptr_t DAT_0204171c;
uintptr_t DAT_02041720;
uintptr_t DAT_02041724;
uintptr_t DAT_02041808;
uintptr_t DAT_0204185c;
uintptr_t DAT_020418ec;
uintptr_t DAT_02041928;
uintptr_t DAT_020419cc;
uintptr_t DAT_02041a38;
uintptr_t DAT_02041a3c;
uintptr_t DAT_02041a40;
uintptr_t DAT_02041a44;
uintptr_t DAT_02041aac;
uintptr_t DAT_02041ab0;
uintptr_t DAT_02041ab4;
uintptr_t DAT_02041ab8;
uintptr_t DAT_02041b5c;
uintptr_t DAT_02041b60;
uintptr_t DAT_02041b64;
uintptr_t DAT_02041b68;
uintptr_t DAT_02041b6c;
uintptr_t DAT_02041b70;
uintptr_t DAT_02041c4c;
uintptr_t DAT_02041c50;
uintptr_t DAT_02041c54;
uintptr_t DAT_02041c58;
uintptr_t DAT_02041c5c;
uintptr_t DAT_02041d74;
uintptr_t DAT_02041de4;
uintptr_t DAT_02042414;
uintptr_t DAT_02042418;
uintptr_t DAT_0204241c;
uintptr_t DAT_02042420;
uintptr_t DAT_02042460;
uintptr_t DAT_02042464;
uintptr_t DAT_020424e0;
uintptr_t DAT_02042560;
uintptr_t DAT_02042564;
uintptr_t DAT_02042568;
uintptr_t DAT_020426a8;
uintptr_t DAT_020427e8;
uintptr_t DAT_020427ec;
uintptr_t DAT_020427f0;
uintptr_t DAT_020427f4;
uintptr_t DAT_020427f8;
uintptr_t DAT_020427fc;
uintptr_t DAT_02042800;
uintptr_t DAT_02042804;
uintptr_t DAT_02042808;
uintptr_t DAT_020429d4;
uintptr_t DAT_020429d8;
uintptr_t DAT_02042a2c;
uintptr_t DAT_02042a7c;
uintptr_t DAT_02042c48;
uintptr_t DAT_02042c4c;
uintptr_t DAT_02042c50;
uintptr_t DAT_02042c54;
uintptr_t DAT_02042d44;
uintptr_t DAT_02042d48;
uintptr_t DAT_02043338;
uintptr_t DAT_02043494;
uintptr_t DAT_0204358c;
uintptr_t DAT_02043590;
uintptr_t DAT_02043594;
uintptr_t DAT_020436d8;
uintptr_t DAT_020436dc;
uintptr_t DAT_020436e0;
uintptr_t DAT_020436e4;
uintptr_t DAT_020436e8;
uintptr_t DAT_0204375c;
uintptr_t DAT_020437a8;
uintptr_t DAT_020437ac;
uintptr_t DAT_020437b0;
uintptr_t DAT_020437b4;
uintptr_t DAT_02043830;
uintptr_t DAT_02043834;
uintptr_t DAT_02043838;
uintptr_t DAT_0204383c;
uintptr_t DAT_02043840;
uintptr_t DAT_02043844;
uintptr_t DAT_02043848;
uintptr_t DAT_0204384c;
uintptr_t DAT_02043850;
uintptr_t DAT_02043854;
uintptr_t DAT_020438d4;
uintptr_t DAT_0204396c;
uintptr_t DAT_02043b4c;
uintptr_t DAT_02043bb4;
uintptr_t DAT_02043bb8;
uintptr_t DAT_02043bec;
uintptr_t DAT_02043bf0;
uintptr_t DAT_02043cfc;
uintptr_t DAT_02043d30;
uintptr_t DAT_02043d78;
uintptr_t DAT_02043e88;
uintptr_t DAT_02043e8c;
uintptr_t DAT_02043e90;
uintptr_t DAT_02043e94;
uintptr_t DAT_02043ed0;
uintptr_t DAT_02043ed4;
uintptr_t DAT_02043f48;
uintptr_t DAT_02043f4c;
uintptr_t DAT_02043fdc;
uintptr_t DAT_02044018;
uintptr_t DAT_020441c0;
uintptr_t DAT_020441c4;
uintptr_t DAT_020441c8;
uintptr_t DAT_020441cc;
uintptr_t DAT_020441d0;
uintptr_t DAT_020441d4;
uintptr_t DAT_020441d8;
uintptr_t DAT_020441dc;
uintptr_t DAT_020441e0;
uintptr_t DAT_020441e4;
uintptr_t DAT_02044348;
uintptr_t DAT_02044464;
uintptr_t DAT_02044468;
uintptr_t DAT_0204446c;
uintptr_t DAT_02044470;
uintptr_t DAT_02044474;
uintptr_t DAT_02044478;
uintptr_t DAT_0204447c;
uintptr_t DAT_020444cc;
uintptr_t DAT_020444d0;
uintptr_t DAT_02044500;
uintptr_t DAT_02044570;
uintptr_t DAT_02044884;
uintptr_t DAT_02044888;
uintptr_t DAT_0204488c;
uintptr_t DAT_02044890;
uintptr_t DAT_02044894;
uintptr_t DAT_02044bac;
uintptr_t DAT_02044fd8;
uintptr_t DAT_020453a4;
uintptr_t DAT_02046ee8;
uintptr_t DAT_02046eec;
uintptr_t DAT_02048550;
uintptr_t DAT_02048614;
uintptr_t DAT_02048c44;
uintptr_t DAT_02048cb8;
uintptr_t DAT_02059e04;
uintptr_t DAT_02059e08;
uintptr_t DAT_02059e0c;
uintptr_t DAT_02059e10;
uintptr_t DAT_02059e14;
uintptr_t DAT_02059e18;
uintptr_t DAT_02059f2c;
uintptr_t DAT_02059f30;
uintptr_t DAT_02059f34;
uintptr_t DAT_02059f38;
uintptr_t DAT_02059f3c;
uintptr_t DAT_02059f40;
uintptr_t DAT_02059f44;
uintptr_t DAT_02059f48;
uintptr_t DAT_02059f4c;
uintptr_t DAT_02059f50;
uintptr_t DAT_02059f54;
uintptr_t DAT_02059f58;
uintptr_t DAT_02059f5c;
uintptr_t DAT_0205a020;
uintptr_t DAT_0205a024;
uintptr_t DAT_0205a028;
uintptr_t DAT_0205a02c;
uintptr_t DAT_0205a030;
uintptr_t DAT_0205a034;
uintptr_t DAT_0205a038;
uintptr_t DAT_0205a03c;
uintptr_t DAT_0205a078;
uintptr_t DAT_0205a07c;
uintptr_t DAT_0205a0d8;
uintptr_t DAT_0205a0dc;
uintptr_t DAT_0205a114;
uintptr_t DAT_0205a184;
uintptr_t DAT_0205a188;
uintptr_t DAT_0205a1d8;
uintptr_t DAT_0205a27c;
uintptr_t DAT_0205a280;
uintptr_t DAT_0205a284;
uintptr_t DAT_040000b0;
uintptr_t DAT_05000000;
uintptr_t DAT_080000b2;
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
/* Forward FUN_02029bf8 (OS_AllocBack) to the real host arena allocator,
 * same as FUN_02029c1c below. */
extern void *OS_Alloc(unsigned int size, unsigned int heap_id);
unsigned int FUN_02029bf8(unsigned int size, unsigned int dir,
                          unsigned int type, unsigned int flag) {
    (void)dir; (void)type; (void)flag;
    static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02029bf8 -> OS_Alloc forwarder\n");}
    void *p = OS_Alloc(size, 0);
    return (unsigned int)(uintptr_t)p;
}
/* Forward FUN_02029c1c (OS_Alloc front wrapper) to the real host arena
 * allocator in arm9/src/heap.c, so callers like FUN_02005b70 receive
 * a pointer that fits in 32 bits and lives inside our VirtualAlloc'd
 * 4 MiB region.  Without this every alloc returned 0 and *DAT_02005d28
 * stayed NULL. */
extern void *OS_Alloc(unsigned int size, unsigned int heap_id);
unsigned int FUN_02029c1c(unsigned int size, unsigned int dir,
                          unsigned int type, unsigned int flag) {
    (void)dir; (void)type; (void)flag;
    static int s; if(!s){s=1; fprintf(stderr,"[stub] FUN_02029c1c -> OS_Alloc forwarder\n");}
    void *p = OS_Alloc(size, 0);
    return (unsigned int)(uintptr_t)p;
}
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
/* FUN_0203b7dc aliased to MI_Fill32 via sdk_symbol_aliases.h */
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
/* FUN_0203e328 resolved via host_nds_fs.c */
/* FUN_0203ed60 resolved via host_nds_fs.c */
/* FUN_0203edcc resolved via host_nds_fs.c */
/* FUN_0203ef08 resolved via host_nds_fs.c */
/* FUN_0203ef50 resolved via host_nds_fs.c */
/* FUN_0203efa0 resolved via host_nds_fs.c */
/* FUN_0203f2a0 resolved via host_nds_fs.c */
/* FUN_0203f2c8 resolved via host_nds_fs.c */
/* FUN_0203f378 resolved via sdk_symbol_aliases.h -> os_timer_dispatch */
/* FUN_0203f398 resolved via host_nds_fs.c */
/* FUN_0203f740 resolved via host_nds_fs.c */
/* FUN_0203f75c resolved via host_nds_fs.c */
/* FUN_0203f864 resolved via host_nds_fs.c */
/* FUN_0203faa8 resolved via host_nds_fs.c */
/* FUN_0203fb40 resolved via host_nds_fs.c */
/* FUN_0203fd64 resolved via host_nds_fs.c */
/* FUN_0203fd8c resolved via host_nds_fs.c */
/* FUN_0203fdd0 resolved via host_nds_fs.c */
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
/* FUN_020739ec — now in host_title_scene.c (next scene constructor) */
/* FUN_02077784 — now in host_title_scene.c (title screen constructor) */
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