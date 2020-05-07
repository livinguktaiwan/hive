#pragma once
#include <hive/chain/steem_fwd.hpp>

#include <fc/uint128.hpp>

#include <hive/chain/steem_object_types.hpp>

#include <hive/protocol/asset.hpp>

namespace hive { namespace chain {

   using hive::protocol::asset;
   using hive::protocol::price;

   /**
    * @class dynamic_global_property_object
    * @brief Maintains global state information
    * @ingroup object
    * @ingroup implementation
    *
    * This is an implementation detail. The values here are calculated during normal chain operations and reflect the
    * current values of global blockchain properties.
    */
   class dynamic_global_property_object : public object< dynamic_global_property_object_type, dynamic_global_property_object>
   {
      public:
         template< typename Constructor, typename Allocator >
         dynamic_global_property_object( Constructor&& c, allocator< Allocator > a )
         {
            c( *this );
         }

         dynamic_global_property_object(){}

         //main HIVE token counter (see also get_full_hive_supply)
         const asset& get_current_supply() const { return current_supply; }

         //initial amount of HBD issued (see also get_full_hbd_supply)
         const asset& get_init_hbd_supply() const { return init_hbd_supply; }
         //main HBD token counter (see also get_full_hbd_supply)
         const asset& get_current_hbd_supply() const { return current_hbd_supply; }
         //rate of interest for holding HBD (in BPS - basis points)
         uint16_t get_hbd_interest_rate() const { return hbd_interest_rate; }
         //percentage of HIVE being converted to HBD during payouts (in BPS - basis points)
         uint16_t get_hbd_print_rate() const { return hbd_print_rate; }

         //pool of HIVE tokens vested normally
         const asset& get_total_vesting_fund_hive() const { return total_vesting_fund_steem; }
         //amount of VESTS produced from HIVE held in normal vested fund
         const asset& get_total_vesting_shares() const { return total_vesting_shares; }

         //pool of HIVE tokens for pending (liquid) rewards
         const asset& get_total_reward_fund_hive() const { return total_reward_fund_steem; }

         //pool of HIVE tokens for pending (vested) rewards
         const asset& get_pending_rewarded_vesting_hive() const { return pending_rewarded_vesting_steem; }
         //amount of VESTS produced from HIVE held in pending reward vested fund
         const asset& get_pending_rewarded_vesting_shares() const { return pending_rewarded_vesting_shares; }

         id_type           id;

         uint32_t          head_block_number = 0;
         block_id_type     head_block_id;
         time_point_sec    time;
         account_name_type current_witness;


         /**
          *  The total POW accumulated, aka the sum of num_pow_witness at the time new POW is added
          */
         uint64_t total_pow = -1;

         /**
          * The current count of how many pending POW witnesses there are, determines the difficulty
          * of doing pow
          */
         uint32_t num_pow_witnesses = 0;

         asset       virtual_supply             = asset( 0, HIVE_SYMBOL );
         asset       current_supply             = asset( 0, HIVE_SYMBOL );
         asset       init_hbd_supply            = asset( 0, HBD_SYMBOL );
         asset       current_hbd_supply         = asset( 0, HBD_SYMBOL );
         asset       total_vesting_fund_steem   = asset( 0, HIVE_SYMBOL );
         asset       total_vesting_shares       = asset( 0, VESTS_SYMBOL );
         asset       total_reward_fund_steem    = asset( 0, HIVE_SYMBOL );
         fc::uint128 total_reward_shares2; ///< the running total of REWARD^2
         asset       pending_rewarded_vesting_shares = asset( 0, VESTS_SYMBOL );
         asset       pending_rewarded_vesting_steem  = asset( 0, HIVE_SYMBOL );

         price       get_vesting_share_price() const
         {
            if ( total_vesting_fund_steem.amount == 0 || total_vesting_shares.amount == 0 )
               return price ( asset( 1000, HIVE_SYMBOL ), asset( 1000000, VESTS_SYMBOL ) );

            return price( total_vesting_shares, total_vesting_fund_steem );
         }

         price get_reward_vesting_share_price() const
         {
            return price( total_vesting_shares + pending_rewarded_vesting_shares,
               total_vesting_fund_steem + pending_rewarded_vesting_steem );
         }

         /**
          *  This property defines the interest rate that HBD deposits receive.
          */
         uint16_t hbd_interest_rate = 0;

         uint16_t hbd_print_rate = HIVE_100_PERCENT;

         /**
          *  Maximum block size is decided by the set of active witnesses which change every round.
          *  Each witness posts what they think the maximum size should be as part of their witness
          *  properties, the median size is chosen to be the maximum block size for the round.
          *
          *  @note the minimum value for maximum_block_size is defined by the protocol to prevent the
          *  network from getting stuck by witnesses attempting to set this too low.
          */
         uint32_t     maximum_block_size = 0;

         /**
          * The size of the block that is partitioned for actions.
          * Required actions can only be delayed if they take up more than this amount. More can be
          * included, but are not required. Block generation should only include transactions up
          * to maximum_block_size - required_actions_parition_size to ensure required actions are
          * not delayed when they should not be.
          */
         uint16_t     required_actions_partition_percent = 0;

         /**
          * The current absolute slot number.  Equal to the total
          * number of slots since genesis.  Also equal to the total
          * number of missed slots plus head_block_number.
          */
         uint64_t      current_aslot = 0;

         /**
          * used to compute witness participation.
          */
         fc::uint128_t recent_slots_filled;
         uint8_t       participation_count = 0; ///< Divide by 128 to compute participation percentage

         uint32_t last_irreversible_block_num = 0;

         /**
          * The number of votes regenerated per day.  Any user voting slower than this rate will be
          * "wasting" voting power through spillover; any user voting faster than this rate will have
          * their votes reduced.
          */
         uint32_t vote_power_reserve_rate = HIVE_INITIAL_VOTE_POWER_RATE;

         uint32_t delegation_return_period = HIVE_DELEGATION_RETURN_PERIOD_HF0;

         uint64_t reverse_auction_seconds = 0;

         int64_t available_account_subsidies = 0;

         uint16_t hbd_stop_percent = 0;
         uint16_t hbd_start_percent = 0;

         //settings used to compute payments for every proposal
         time_point_sec next_maintenance_time;
         time_point_sec last_budget_time;

         uint16_t content_reward_percent = HIVE_CONTENT_REWARD_PERCENT_HF16;
         uint16_t vesting_reward_percent = HIVE_VESTING_FUND_PERCENT_HF16;
         uint16_t sps_fund_percent = HIVE_PROPOSAL_FUND_PERCENT_HF0;

         asset sps_interval_ledger = asset( 0, HBD_SYMBOL );

         uint16_t downvote_pool_percent = 0;

#ifdef HIVE_ENABLE_SMT
         asset smt_creation_fee = asset( 1000, HBD_SYMBOL );
#endif
   };

   typedef multi_index_container<
      dynamic_global_property_object,
      indexed_by<
         ordered_unique< tag< by_id >,
            member< dynamic_global_property_object, dynamic_global_property_object::id_type, &dynamic_global_property_object::id > >
      >,
      allocator< dynamic_global_property_object >
   > dynamic_global_property_index;

} } // hive::chain

#ifdef ENABLE_MIRA
namespace mira {

template<> struct is_static_length< hive::chain::dynamic_global_property_object > : public boost::true_type {};

} // mira
#endif

FC_REFLECT( hive::chain::dynamic_global_property_object,
             (id)
             (head_block_number)
             (head_block_id)
             (time)
             (current_witness)
             (total_pow)
             (num_pow_witnesses)
             (virtual_supply)
             (current_supply)
             (init_hbd_supply)
             (current_hbd_supply)
             (total_vesting_fund_steem)
             (total_vesting_shares)
             (total_reward_fund_steem)
             (total_reward_shares2)
             (pending_rewarded_vesting_shares)
             (pending_rewarded_vesting_steem)
             (hbd_interest_rate)
             (hbd_print_rate)
             (maximum_block_size)
             (required_actions_partition_percent)
             (current_aslot)
             (recent_slots_filled)
             (participation_count)
             (last_irreversible_block_num)
             (vote_power_reserve_rate)
             (delegation_return_period)
             (reverse_auction_seconds)
             (available_account_subsidies)
             (hbd_stop_percent)
             (hbd_start_percent)
             (next_maintenance_time)
             (last_budget_time)
             (content_reward_percent)
             (vesting_reward_percent)
             (sps_fund_percent)
             (sps_interval_ledger)
             (downvote_pool_percent)
#ifdef HIVE_ENABLE_SMT
             (smt_creation_fee)
#endif
          )
CHAINBASE_SET_INDEX_TYPE( hive::chain::dynamic_global_property_object, hive::chain::dynamic_global_property_index )
