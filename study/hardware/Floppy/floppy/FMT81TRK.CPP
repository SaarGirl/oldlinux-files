#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <errno.h>

typedef struct _EBPB_
{
  unsigned sectsize;
  char clustsize;
  unsigned ressecs;
  char fatcnt;
  unsigned rootsize;
  unsigned totsecs;
  char media;
  unsigned fatsize;
  unsigned seccnt;
  unsigned headcnt;
  unsigned hiddensec_low;
  unsigned hiddensec_hi;
  unsigned long drvsecs;
} EBPB;

typedef struct _TRK_LY_
{
  unsigned no;
  unsigned size;
} TRK_LY;

typedef struct _DPB_
{
  char spec;
  char devtype;
  unsigned devattr;
  unsigned numofcyl;
  char media_type;
  EBPB bpb;
  char reserved[6];
  unsigned trkcnt;
  TRK_LY trk[100];
} DPB;

typedef struct _DPB_FORMAT_
{
  char spec;
  unsigned head;
  unsigned track;
} DPB_FORMAT;

int main(void)
{
  union REGS reg;
  struct SREGS segreg;
  DPB far *dbp;
  DPB_FORMAT far *dbp_f;
  int sectors, i;

  printf("\n�ணࠬ�� 㭨�⮦�� ᮤ�ন���"
   "\n81-� ��஦�� ��᪠ �:."
   "\n������ �த������? (Y,N)\n");

  i = getch();
  if((i != 'y') && (i != 'Y'))
    return(-1);

  // �����뢠�� ������ ��� ����� ��ࠬ��஢ ���ன�⢠
  dbp = (DPB far*)farmalloc(sizeof(DPB));

  // �����뢠�� ������ ��� ����� ��ࠬ��஢ ���ன�⢠,
  // ����� �㤥� �ᯮ�짮��� ��� �ଠ�஢����
  dbp_f = (DPB_FORMAT far*)
    farmalloc(sizeof(DPB_FORMAT));

  if(dbp == NULL || dbp_f == NULL)
  {
    printf("\n���� �����");
    return(-1);
  }

  // ����砥� ⥪�騥 ��ࠬ���� ��᪠ �:
  dbp->spec = 0;
  reg.x.ax = 0x440d;
  reg.h.bl = 1;
  reg.x.cx = 0x0860;
  reg.x.dx =  FP_OFF(dbp);
  segreg.ds = FP_SEG(dbp);
  intdosx(&reg, &reg, &segreg);

  if(reg.x.cflag != 0)
  {
    printf("\n�訡��: %d", reg.x.ax);
    return(-1);
  }

  // ������塞 ���� ��ଥ�஢ ��� �ଠ�஢����
  dbp->spec = 5;

  // ���뢠�� �� BPB ������⢮ ᥪ�஢ �� ��஦��
  sectors = dbp->bpb.seccnt;

  // �����⠢������ ⠡����, ����뢠���� �ଠ� ��஦��

  // �����뢠�� ������⢮ ᥪ�஢ �� ��஦��
  dbp->trkcnt = sectors;

  // ��� ������� ᥪ�� �� ��஦�� � ⠡����
  // �����뢠�� ��� ����� � ࠧ���.
  for(i = 0; i < sectors; i++)
  {
    dbp->trk[i].no   = i+1;
    dbp->trk[i].size = 512;
  }

  // ��⠭�������� ���� ��ࠬ���� ��� ��᪠ �:
  reg.x.ax = 0x440d;
  reg.h.bl = 1;
  reg.x.cx = 0x0840;
  reg.x.dx =  FP_OFF(dbp);
  segreg.ds = FP_SEG(dbp);
  intdosx(&reg, &reg, &segreg);

  if(reg.x.cflag != 0)
  {
    printf("\n�訡��: %d", reg.x.ax);
    return(-1);
  }

  // ��⮢�� ���� ��ࠬ��஢ ���ன�⢠,
  // ����� �㤥� �ᯮ�짮��� �� �맮��
  // ����樨 �஢�ન ���������� �ଠ�஢����
  // ��஦��
  dbp_f->spec = 1;
  dbp_f->head = 0;
  dbp_f->track = 81;

  reg.x.ax = 0x440d;
  reg.h.bl = 1;
  reg.x.cx = 0x0842;
  reg.x.dx =  FP_OFF(dbp_f);
  segreg.ds = FP_SEG(dbp_f);
  intdosx(&reg, &reg, &segreg);

  if(reg.x.cflag != 0)
  {
    printf("\n�訡��: %d", reg.x.ax);
    return(-1);
  }

  // �᫨ 㪠����� �ଠ� ��஦�� �����ন������,
  // ���� ᯥ樠���� �㭪権 �㤥� ᮤ�ঠ�� 0.
  // �஢��塞 ��
  if(dbp_f->spec != 0)
  {
    printf("\n��ଠ� ��஦�� �� �����ন������");
    return(-1);
  }

  // ������塞 ���� ��ࠬ��஢ ��� �믮������
  // ����樨 �ଠ�஢����
  dbp_f->spec = 0;
  dbp_f->head = 0;
  dbp_f->track = 81;

  // ��ଠ��㥬 ��஦�� � ����஬ 81, ������� 0
  reg.x.ax = 0x440d;
  reg.h.bl = 1;
  reg.x.cx = 0x0842;
  reg.x.dx =  FP_OFF(dbp_f);
  segreg.ds = FP_SEG(dbp_f);
  intdosx(&reg, &reg, &segreg);

  if(reg.x.cflag != 0)
  {
    printf("\n�訡��: %d", reg.x.ax);
    return(-1);
  }

  // �᢮������� ������
  farfree(dbp);
  farfree(dbp_f);

  return(0);
}

