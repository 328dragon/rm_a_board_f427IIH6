#include "dm_j4310.h"

DM_J4310_Controller_t *DM_J4310_instnce[DM_J4310_NUM] = {NULL}; // dm_j4310实例数组
static int dm_idx = 0;                                          // dm_j4310实例索引,每次有新的模块注册会自增

void DM_J4310_Get_Info(CANInstance *can_instance)
{
    ((DM_J4310_Controller_t *)(can_instance->id))->dm_imfo_instance.Master_id = (can_instance->rx_buff[0] >> 4);
    ((DM_J4310_Controller_t *)(can_instance->id))->dm_imfo_instance.Erro = (can_instance->rx_buff[0] << 4);
    ((DM_J4310_Controller_t *)(can_instance->id))->motor_instnce.get.deg_pos = (((int16_t)(can_instance->rx_buff[1])) << 8) + can_instance->rx_buff[2];
    ((DM_J4310_Controller_t *)(can_instance->id))->motor_instnce.get.velocity = (((int16_t)(can_instance->rx_buff[3])) << 4 )+ (can_instance->rx_buff[4] >> 4);
    ((DM_J4310_Controller_t *)(can_instance->id))->dm_imfo_instance.Torque = (((int16_t)(can_instance->rx_buff[4])) << 8 )+ can_instance->rx_buff[5];
    ((DM_J4310_Controller_t *)(can_instance->id))->dm_imfo_instance.T_Mos = can_instance->rx_buff[6];
    ((DM_J4310_Controller_t *)(can_instance->id))->dm_imfo_instance.T_Rotor = can_instance->rx_buff[7];
}

void DM_4310_Register(CAN_HandleTypeDef *hcan, uint32_t protocol_id, uint32_t mst_id, uint16_t w_mode)
{
    DM_J4310_Controller_t *DM_J4310_s = (DM_J4310_Controller_t *)malloc(sizeof(DM_J4310_Controller_t));
    DM_J4310_s->mode = w_mode;
    DM_J4310_s->protocol_id = protocol_id;
    CAN_Init_Config_s can_instance_config = {0};
    memset(DM_J4310_s, 0, sizeof(DM_J4310_Controller_t));
    can_instance_config.can_handle = hcan;
    can_instance_config.can_module_callback = DM_J4310_Get_Info; // 这里可以设置回调函数,但是目前没有用到
    can_instance_config.id = DM_J4310_s;
    can_instance_config.rx_id = mst_id; // 返回帧id
    if (w_mode == mit_mode)
        DM_J4310_s->mode_trans_id = protocol_id + 0x000;
    else if (w_mode == pos_vel_mode)
        DM_J4310_s->mode_trans_id = protocol_id + 0x100;
    else if (w_mode == vel_mode)
        DM_J4310_s->mode_trans_id = protocol_id + 0x200;

    can_instance_config.tx_id = DM_J4310_s->mode_trans_id;
    DM_J4310_s->can_instance = CANRegister(&can_instance_config); // 注册CAN实例

    DM_J4310_instnce[dm_idx++] = DM_J4310_s;
}
void DM_Change_Mode(DM_J4310_Controller_t *dm_j4310_instance, uint8_t to_mode)
{
    dm_j4310_instance->mode = to_mode; // 设置新的工作模式
    if (to_mode==mit_mode)
   {
    dm_j4310_instance->mode_trans_id=dm_j4310_instance->protocol_id+0x000;
   } 
    else if (to_mode==pos_vel_mode)
    {
        dm_j4310_instance->mode_trans_id=dm_j4310_instance->protocol_id+0x100;
    }
    else if (to_mode==vel_mode)
    {
        dm_j4310_instance->mode_trans_id=dm_j4310_instance->protocol_id+0x200;
    }
    dm_j4310_instance->can_instance->tx_id=dm_j4310_instance->mode_trans_id;

}
void Enable_DM(DM_J4310_Controller_t *dm_j4310_instance)
{
    uint8_t motor_data[8] = {0}; // 发送数据缓存
    motor_data[0] = 0xFF;
    motor_data[1] = 0xFF;
    motor_data[2] = 0xFF;       
    motor_data[3] = 0xFF;        // 使能电机
    motor_data[4] = 0xFF;        // 使能电机
    motor_data[5] = 0xFF;        // 使能电机
    motor_data[6] = 0xFF;        // 使能电机
    motor_data[7] = 0xFC;        // 使能电机
    memcpy(dm_j4310_instance->can_instance->tx_buff, motor_data, 8); // 将数据拷贝到CAN实例的发送缓存中
    CANTransmit(dm_j4310_instance->can_instance);                    // 发送数据
}
void Control_DM(DM_J4310_Controller_t *dm_j4310_instance)
{
    uint8_t motor_data[8] = {0}; // 发送数据缓存


    if (dm_j4310_instance->mode == mit_mode)
    {
      motor_data[0] = dm_j4310_instance->dm_controller_instance.p_des >> 8; 
      motor_data[1] = (uint8_t)dm_j4310_instance->dm_controller_instance.p_des;
        motor_data[2] = dm_j4310_instance->dm_controller_instance.v_des >> 4;  
        motor_data[3]= (dm_j4310_instance->dm_controller_instance.v_des>>8)+(dm_j4310_instance->dm_controller_instance.Kp>>4);
        motor_data[4]=(uint8_t) dm_j4310_instance->dm_controller_instance.Kp ;
        motor_data[5]=dm_j4310_instance->dm_controller_instance.Kd>>4;
        motor_data[6]=((uint8_t)dm_j4310_instance->dm_controller_instance.Kd)&0x0000FFFF+(dm_j4310_instance->dm_controller_instance.t_ff>>8);
        motor_data[7]=(uint8_t) dm_j4310_instance->dm_controller_instance.t_ff;

    }
    else if (dm_j4310_instance->mode == pos_vel_mode)
    {
        for(int i=0;i<=3;i++)
        {
            motor_data[i] = (uint8_t)dm_j4310_instance->dm_controller_instance.p_des; // 位置设定值高8位
        }
        for(int i=4;i<=7;i++)
        {
            motor_data[i] = (uint8_t)dm_j4310_instance->dm_controller_instance.v_des;
        }
    }
    else if (dm_j4310_instance->mode == vel_mode)
    {
        for(int i=0;i<=3;i++)
        {
            motor_data[i] = (uint8_t)dm_j4310_instance->dm_controller_instance.v_des; // 位置设定值高8位
        }
        for(int i=4;i<=7;i++)
        {
            motor_data[i] =0;
        }
    }

    memcpy(dm_j4310_instance->can_instance->tx_buff, motor_data, 8); // 将数据拷贝到CAN实例的发送缓存中
    CANTransmit(dm_j4310_instance->can_instance);                    // 发送数据
}