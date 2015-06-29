#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include <QMap>


//const   TCauseValue     ReleaseCause_Unallocated_number                           =   1;
//const   TCauseValue     ReleaseCause_No_route_to_transit_network                  =   2;
//const   TCauseValue     ReleaseCause_No_route_to_destination                      =   3;
//const   TCauseValue     ReleaseCause_Channel_unacceptable                         =   6;
//const   TCauseValue     ReleaseCause_Call_awarded                                 =   7;
//const   TCauseValue     ReleaseCause_Preemption                                   =   8;  // SS-MLPP Q.955
//const   TCauseValue     ReleaseCause_Normal_call_clearing                         =  16;
//const   TCauseValue     ReleaseCause_User_busy                                    =  17;
//const   TCauseValue     ReleaseCause_No_user_responding                           =  18;
//const   TCauseValue     ReleaseCause_No_answer_from_user                          =  19;
//const   TCauseValue     ReleaseCause_Subscriber_absent                            =  20;
//const   TCauseValue     ReleaseCause_Call_rejected                                =  21;
//const   TCauseValue     ReleaseCause_Number_changed                               =  22;
//const   TCauseValue     ReleaseCause_Exchange_routing_error                       =  25;
//const   TCauseValue     ReleaseCause_Non_selected_user_clearing                   =  26;
//const   TCauseValue     ReleaseCause_Destination_out_of_order                     =  27;
//const   TCauseValue     ReleaseCause_Invalid_number_format                        =  28;
//const   TCauseValue     ReleaseCause_Facility_rejected                            =  29;
//const   TCauseValue     ReleaseCause_Response_to_STATUS_ENQUIRY                   =  30;
//const   TCauseValue     ReleaseCause_Normal_unspecified                           =  31;
//const   TCauseValue     ReleaseCause_No_circuit_channel_available                 =  34;
//const   TCauseValue     ReleaseCause_Network_out_of_order                         =  38;
//const   TCauseValue     ReleaseCause_Temporary_failure                            =  41;
//const   TCauseValue     ReleaseCause_Switching_equipment_congestion               =  42;
//const   TCauseValue     ReleaseCause_Access_information_discarded                 =  43;
//const   TCauseValue     ReleaseCause_Reqsted_channel_not_available                =  44;
//const   TCauseValue     ReleaseCause_Precedence_call_blocked                      =  46;  // SS-MLPP Q.955
//const   TCauseValue     ReleaseCause_Resource_unavailable_unspecif                =  47;
//const   TCauseValue     ReleaseCause_Quality_of_service_unavailable               =  49;
//const   TCauseValue     ReleaseCause_Reqsted_facility_not_subscribed              =  50;
//const   TCauseValue     ReleaseCause_Bearer_capability_not_authrzed               =  57;
//const   TCauseValue     ReleaseCause_Bearer_capability_not_available              =  58;
//const   TCauseValue     ReleaseCause_Service_not_available_unspec                 =  63;
//const   TCauseValue     ReleaseCause_Bearer_capability_not_implmted               =  65;
//const   TCauseValue     ReleaseCause_Channel_type_not_implemented                 =  66;
//const   TCauseValue     ReleaseCause_Reqsted_facility_not_implmted                =  69;
//const   TCauseValue     ReleaseCause_Only_restricted_digital_bearer               =  70;
//const   TCauseValue     ReleaseCause_Service_not_implemented                      =  79;
//const   TCauseValue     ReleaseCause_Invalid_call_reference_value                 =  81;
//const   TCauseValue     ReleaseCause_Identified_channel_not_exist                 =  82;
//const   TCauseValue     ReleaseCause_No_call_ID_for_suspended_call                =  83;
//const   TCauseValue     ReleaseCause_Call_identity_in_use                         =  84;
//const   TCauseValue     ReleaseCause_No_call_suspended                            =  85;
//const   TCauseValue     ReleaseCause_Call_has_been_cleared                        =  86;
//const   TCauseValue     ReleaseCause_User_not_a_member_of_CUG                     =  87;
//const   TCauseValue     ReleaseCause_Non_existent_closed_user_group               =  88;
//const   TCauseValue     ReleaseCause_Called_user_not_member_of_GUG                =  89;
//const   TCauseValue     ReleaseCause_Called_user_belongs_to_GUG                   =  90;
//const   TCauseValue     ReleaseCause_Invalid_transit_network                      =  91;
//const   TCauseValue     ReleaseCause_Invalid_message_unspecified                  =  95;
//const   TCauseValue     ReleaseCause_Mandatory_IE_is_missing                      =  96;
//const   TCauseValue     ReleaseCause_Messsage_type_non_existent                   =  97;
//const   TCauseValue     ReleaseCause_Msg_uncompatible_non_existent                =  98;
//const   TCauseValue     ReleaseCause_IE_non_existent                              =  99;
//const   TCauseValue     ReleaseCause_Invalid_IE_contents                          = 100;
//const   TCauseValue     ReleaseCause_Message_not_compatible                       = 101;
//const   TCauseValue     ReleaseCause_Recovery_on_timer_expiry                     = 102;
//const   TCauseValue     ReleaseCause_Protocol_error_unspecified                   = 111;
//const   TCauseValue     ReleaseCause_Interworking_unspecified                     = 127;


class QCDRTableModel : public QSqlTableModel
{
private:
    int getIntTypeCalls(const QModelIndex &index) const; //
    QString getQStringTypeCalls(int value) const;


public:
    QCDRTableModel();
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    ~QCDRTableModel();

    static const unsigned char COL_IN_TYPE    = 0;
    static const unsigned char COL_IN_MN      = 1; // In module number
    static const unsigned char COL_IN_SLPCM   = 2; // In slot/pcm number
    static const unsigned char COL_IN_PORT    = 3; // In port number
    static const unsigned char COL_IN_AON     = 4; // In AON number
    static const unsigned char COL_IN_NUMBER  = 5; // In Number
    static const unsigned char COL_OUT_TYPE   = 6;
    static const unsigned char COL_OUT_MN     = 7;
    static const unsigned char COL_OUT_SLPCM  = 8;
    static const unsigned char COL_OUT_PORT   = 9;
    static const unsigned char COL_OUT_AON    = 10;
    static const unsigned char COL_OUT_NUMBER = 11;
    static const unsigned char COL_DATE       = 12;
    static const unsigned char COL_TIME       = 13;
    static const unsigned char COL_CALL_TYPE  = 14;
    static const unsigned char COL_TIME_SEIZ  = 15;
    static const unsigned char COL_TIME_TALK  = 16;
    static const unsigned char COL_CRELEASE   = 17;


    static QMap <unsigned char, QString> causeValue;
};

#endif // MYTABLEMODEL_H
