/*
 *  TOPPERS/HRMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable Multiprocessing Profile Kernel
 * 
 *  Copyright (C) 2020-2021 by TOPPERS Project, JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  $Id: target_class.cdf 3242 2021-11-21 12:50:02Z okuma-top $
 */

/*
 *  TECS向けのクラス定義
 *
 *  regionのclass指定子で使用可能なクラス名を定義する．HRMPClassPlugin
 *  でのチェックにより使用される．
 *
 *  class_name: TECSにおけるクラス名
 *  processorID: プロセッサID
 *  locality: 以下の3つのいずれか
 *		"only"
 *			処理単位セル … processorIDで指定されたプロセッサでのみ動作
 *			非処理単位セル … processorIDで指定されたプロセッサでのみ動作
 *		"all"
 *			処理単位セル … 起動時はprocessorIDで指定されたプロセッサで
 *							動作．マイグレート可能
 *			非処理単位セル … processorIDで指定されたプロセッサからは高
 *							  速にアクセスできる．それ以外のプロセッサか
 *							  らは低速になる
 *		"root"
 *			処理単位セル … 一部例外を除き配置できない
 *			非処理単位セル … どのプロセッサからも一様にアクセスできる．
 *							  カーネルオブジェクトのセルは配置できない
 *          root はクラス指定されていないリージョンを表す（TECSジェネレータで既定）
 *  class_name_in_kernel: HRMP3でのクラス名
 *						  class_nameと一致する場合には省略可能
 */
__tool_info__ ("HRMP_class_def") {
	"class_def" : [
		{
			"type"			: "class",
			"class_name"	: "CLS_PRC1",
			"processorID"	: 1,
			"locality"		: "only"
		},
		{
			"type"			: "class",
			"class_name" 	: "CLS_ALL_PRC1",
			"processorID"	: 1,
			"locality"		: "all"
		},
		{
			"type"			: "class",
			"class_name" 	: "CLS_PRC2",
			"processorID"	: 2,
			"locality"		: "only"
		},
		{
			"type"			: "class",
			"class_name"	: "CLS_ALL_PRC2",
			"processorID"	: 2,
			"locality"		: "all"
		}
	]
}
